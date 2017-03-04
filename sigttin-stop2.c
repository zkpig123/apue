//1.test sigttin when background process read
//from stand input
//2.test method: run progname in background, and
//using ps to check status of process
//conclude: even blocking signal SIGTTIN, read from 
//terminal from background process will be interrupted
//with errno set to EIO

#include "assist.h"

int main (void)
{
	int buf[5];
	int fd = open("test-sigttin2", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1) p_err("open failed.");
	errno = 0;
	char *error;

	sigset_t set;
	if (sigemptyset(&set)) err("sigemptyset.");
	if (sigaddset(&set, SIGTTIN)) err("sigaddset.");
	if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) p_err("sigprocmask");
	printf("SIGTTIN is blocked.\n");

	errno = 0;
	if (read(STDIN_FILENO, buf, 1) != 1) write(fd, error=strerror(errno), strlen(error));

	return 0;
}
