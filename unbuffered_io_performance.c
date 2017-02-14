//usage: progname filename read|write buffer_size

#include "assist.h"
#define TMP_FILE_NAME "tmp23kfff"
#define MAX_WRITE_SIZE 2684354560L

int main (int argc, char *argv[])
{
	int64_t bufsiz;
	struct stat file_status;
	char *buf;
	if (argc != 4) err ("usage: progname filename read|write buffer_size");
	if (strchr (argv[1], '/')) err ("filename should not contain '/'");
	if ((bufsiz = atoll (argv[3])) <= 0) err ("buffer size invalid.");
	if ((buf = malloc (bufsiz)) == NULL) err ("malloc failed.");

	if (stat (".", &file_status) == -1) p_err ("stat failed.");
	printf ("File system block size is %ld\n", file_status.st_blksize);
	int fd, tmp_fd;
	ssize_t bytes;
	ssize_t written_bytes = 0;

	if (strcmp (argv[2], "read") == 0){
		if ((fd = open (argv[1], O_RDONLY | O_SYNC)) == -1) p_err ("open failed.");
		printf ("read, bufsiz:%ld\n", bufsiz);
		while ((bytes = read (fd, buf, bufsiz)) > 0) ;;
		if (bytes < 0) p_err ("read failed.");
	}
	else if (strcmp (argv[2], "write") == 0){
		if ((tmp_fd = open (TMP_FILE_NAME, O_CREAT | O_EXCL | O_WRONLY, S_IWUSR | S_IRUSR)) == -1) p_err ("open failed.");
		if (unlink (TMP_FILE_NAME) == -1) p_err ("unlink failed.");
		printf ("write, bufsiz:%ld\n", bufsiz); 
		while (written_bytes < MAX_WRITE_SIZE){
			if ((bytes = write (tmp_fd, buf, bufsiz)) < 0) p_err ("write failed.");
			else written_bytes += bytes;
		}
	}
	else{
		err ("operation should be read or write.");
	}

	return 0;
}
