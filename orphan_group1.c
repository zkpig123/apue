//conclude: if any member of newly orphan group
//stop, then SIGHUP followed by SIGCONT is sent
//to every member of that group

#include "assist.h"

void sig_handler (int signo)
{
	printf("pid:%ld, signal catched, signal number:%d\n", getpid(), signo); 
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if (sigaction(SIGHUP, &act, NULL) == -1) p_err("sigaction 4 SIGHUP failed.");
	if (sigaction(SIGCONT, &act, NULL) == -1) p_err("sigaction 4 SIGCONT failed.");

	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork 1 err.");
	if (pid == 0){
		pid = getpid();
		while (1){
			sleep(1);
			printf("child: pid:%ld\n", pid);
		}
		return 0;
	}
	if ((pid = fork()) == -1) p_err("fork 2 err.");
	if (pid){
		sleep(3);
		return 0;
	}else{
		kill(getpid(), SIGTSTP); //alternatively SIGSTOP
		return 0;
	}

	return 0;
}
