//The maximum size of files that the process may create. Attempts to extend a file beyond this limit result in delivery of a SIGXFSZ signal. By default, this signal terminates a process, but a process can catch this signal instead, in which case the relevant system call (e.g., write(2), truncate(2)) fails with the error EFBIG.

#include "assist.h"

void sig_handler (int signo)
{
	printf("signal %ld catched.\n", signo);
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if(sigaction(SIGXFSZ, &act, NULL) == -1) p_err("sigaction 4 SIGXFSZ failed.");
	struct rlimit rlim;
	if (getrlimit(RLIMIT_FSIZE, &rlim) == -1) p_err("getrlimit failed.");
	printf("RLIMIT_FSIZE: soft:%ld, hard:%ld\n", rlim.rlim_cur, rlim.rlim_max);
	printf("set RLIMIT_FSIZE soft limit to 1, hard limit to 1.\n");
	rlim.rlim_max = 1;
	rlim.rlim_cur = 1;
	if (setrlimit(RLIMIT_FSIZE, &rlim) == -1) p_err("setrlimit failed.");
	printf("now RLIMIT_FSIZE: soft:%ld, hard:%ld\n", rlim.rlim_cur, rlim.rlim_max);

	int fd;
	if ((fd = open("sigxfsz.test", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");
	errno = 0;
	for (int i = 0; i < 5; i++){
		sleep(1);
		if (write(fd, "a", 1) != 1){
			if (errno) perror("write failed.");
		}
	}

	return 0;
}
