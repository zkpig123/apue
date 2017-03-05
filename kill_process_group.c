#include "assist.h"

void sig_handler (int signo)
{
	printf("pid:%ld, signal %ld catched.\n\n", getpid(), signo);
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if(sigaction(SIGTERM, &act, NULL) == -1) p_err("sigaction 4 SIGTERM failed.");

	printf("parent pid:%ld\n", getpid());
	pid_t pid1;
	if ((pid1 = fork()) == -1) p_err("fork failed.");
	if (pid1 == 0){
		if (setpgid(0, 0) == -1) p_err("setpgid failed.");
		printf("child 1 pid:%ld, pgrp:%ld\n", getpid(), getpgrp());
		while (1);
	}
	pid_t pid2;
	if ((pid2 = fork()) == -1) p_err("fork failed.");
	if (pid2 == 0){
		if (setpgid(0, 0) == -1) p_err("setpgid failed.");
		printf("child 2 pid:%ld, pgrp:%ld\n", getpid(), getpgrp());
		while (1);
	}
	pid_t pid3;
	if ((pid3 = fork()) == -1)p_err("fork failed.");
	if (pid3 == 0){
		printf("child 3 pid:%ld, pgrp:%ld\n", getpid(), getpgrp());
		while (1);
	}

	sleep(1);

	printf("send SIGTERM to child 1.\n");
	kill(pid1, SIGTERM);
	sleep(1);

	printf("send SIGTERM to child 2.\n");
	kill(pid2, SIGTERM);
	sleep(1);

	printf("send SIGTERM to child 3.\n");
	kill(pid3, SIGTERM);
	sleep(1);

	printf("press any to invoke kill to send SIGTERM to process group of the caller and all processes that permission to send.\n");
	getchar();
	kill(0, SIGTERM);

	sleep(1);
	printf("press any to invoke kill to send SIGTERM to another process group not equal to caller and all processes that permission to send.\n");
	getchar();
	kill(-(long)pid2, SIGTERM);

	sleep(1);
	printf("press any to invoke kill to send SIGTERM to all processes that has permission to send.\n");
	getchar();
	kill(-1, SIGTERM);

	sleep(3);
	printf("press any key to exit.\n");
	getchar();
	kill(pid1, SIGKILL);
	kill(pid2, SIGKILL);
	kill(pid3, SIGKILL);
	return 0;
}
