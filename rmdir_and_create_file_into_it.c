//1.usage: progname directory_name

#include "assist.h"

extern int errno;

int main (int argc, char *argv[])
{
	int dir_fd;
	if (argc != 2) err ("usage: progname directory_name.");
	if ((dir_fd = open (argv[1], O_RDONLY)) == -1) p_err ("open failed.");
	if (rmdir (argv[1]) == -1) p_err ("rmdir failed.");
	printf ("press any key to continue..\n");
	getchar ();
	if (openat (dir_fd, "test", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR) == -1){
		printf ("openat to create file into dir %s failed.\n", argv[1]);
		perror ("error:");
	}

	return 0;
}
