//1.usage: progname directory_name
//2.conclude: read can't operate on dir descriptor,
//should use fdopendir to get DIR structure from
//file descriptor of directory to do this

#include "assist.h"

#ifndef BUFSIZ
#define BUFSIZ 4096
#endif

int main (int argc, char *argv[])
{
	if (argc != 2) err ("usage: progname directory_name.");
	int dir_fd;
	if ((dir_fd = open (argv[1], O_RDONLY)) == -1) p_err ("open failed.");
	char buf[BUFSIZ];
	ssize_t bytes;
	while ((bytes = read (dir_fd, buf, BUFSIZ)) > 0){
		if (write (STDOUT_FILENO, buf, BUFSIZ) == -1) p_err ("write to STDOUT_FILENO failed.");
	}
	if (bytes == -1) p_err ("read failed.");

	return 0;
}
