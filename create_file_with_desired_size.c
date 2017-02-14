//usage: progname file_path_to_create file_size

#include "assist.h"
#define BUFLEN 4096

int main (int argc, char *argv[])
{
	int fd;
	int64_t file_size;
	char buf[BUFLEN];

	if (argc != 3) err ("usage: progname file_path_to_create file_size.");
	if ((file_size = atoll (argv[2])) <= 0) err ("file size invalid.");
	memset (buf, 0, BUFLEN);

	umask (0);
	if ((fd = open (argv[1], O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) p_err ("open failed.");
	int current_loops = 1;
	int total_loops = file_size / BUFLEN;
	int remainder_bytes = file_size % BUFLEN;
	while (current_loops <= total_loops){
		if (write (fd, buf, BUFLEN) == -1) p_err ("write failed.");
		current_loops++;
	}
	if (write (fd, buf, remainder_bytes) == -1) p_err ("write remainder failed.");
	close (fd);
}
