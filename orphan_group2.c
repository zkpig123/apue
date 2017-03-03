//conclude: if member of an aged orphan group (which is not an newly 
//orphan group) stopped, SIGHUP(followed by SIGCONT) is not! sent 
//to member of that group.
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
	if (sigaction(SIGTERM, &act, NULL) == -1) p_err("sigaction 4 SIGTERM failed.");

	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork 1 err.");
	if (pid == 0){
		pid = getpid();
		while (2){
			printf("child 1 (pid:%ld) running.\n");
			sleep(2);
		}
		return 0;
	}
	pid_t ppid = getpid();
	if ((pid = fork()) == -1) p_err("fork 2 err.");
	if (pid){
		sleep(2);
		printf("parent(pid:%ld) now kill child2 with sigterm to awake it and exit and become orphaned group\n", (long)ppid);
		kill(pid, SIGTERM);
		return 0;
	}else{
		printf("child2(pid:%ld): \n", getpid());
		pause();
		printf("child2(pid:%ld) after 2 seconds kill itself with sigtstp.\n", getpid());
		sleep(2);
		if (kill(getpid(), SIGTSTP) == -1) p_err("kill child2 itself failed.");
		return 0;
	}

	return 0;
}
