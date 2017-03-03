#include "assist.h"

void sig_hup (int signo)
{
	printf("signal %ld catched.\n", signo);
}

int main (void)
{
	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork 1 failed.");
	if (pid){
		sleep(2);
		return 0; //parent exit
	}
	if ((pid = fork()) == -1) p_err("fork 2 failed.");
	if (pid){
		pid_t sess_id;
		if ((sess_id = setsid()) == (pid_t)-1) p_err("setsid failed.");
		while (1){
			sleep(1);
			printf("parent: pid:%ld\n", (long)getpid());
		}
	}
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_hup;
	if (sigaction(SIGHUP, &act, NULL) == -1) p_err("sigaction failed.");
	kill(getpid(), SIGTSTP);

	return 0;
}
