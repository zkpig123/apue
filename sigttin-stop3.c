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
	int fd = open("test-sigttin3", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1) p_err("open failed.");
	errno = 0;
	char *error;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	if(sigaction(SIGTTIN, &act, NULL) == -1) p_err("sigaction 4 sigttin fialed.");
	printf("SIGTTIN is ignored.\n");

	errno = 0;
	if (read(STDIN_FILENO, buf, 1) != 1) write(fd, error=strerror(errno), strlen(error));

	return 0;
}
