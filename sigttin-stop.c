//1.test sigttin when background process read
//from stand input
//2.test method: run progname in background, and
//using ps to check status of process

#include "assist.h"

int main (void)
{
	int buf[5];
	int fd = open("test-sigttin", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1) p_err("open failed.");
	errno = 0;
	char *error;
	if (read(STDIN_FILENO, buf, 1) != 1) write(fd, error=strerror(errno), strlen(error));

	return 0;
}
