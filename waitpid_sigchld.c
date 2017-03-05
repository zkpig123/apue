//POSIX.1 states that if wait or waitpid 
//returns the status of a child process 
//while SIGCHLD is pending, then SIGCHLD 
//should not be delivered to the process 
//unless the status of another child process 
//is also available. Linux 3.2.0, however, 
//doesn’t —SIGCHLD remains pending after the 
//system function calls waitpid !!! when the 
//signal is unblocked, it is delivered to the 
//caller. If we called wait in the sig_chld 
//function in Figure 10.26, a Linux system 
//would return −1 with errno set to ECHILD

#include "assist.h"

void sig_handler (int signo)
{
	int status;
	printf("signal %d catched.\n", signo);
	if (waitpid(-1, &status, 0) == -1){
		perror("waitpid failed.");
	}
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if(sigaction(SIGCHLD, &act, NULL) == -1) p_err("sigaction 4 SIGCHLD failed.");
	pid_t pid;
	sigset_t newset, oldset;
	if (sigemptyset(&newset)) err("sigemptyset err.");
	if (sigaddset(&newset, SIGCHLD)) err("sigaddset err.");
	if (sigprocmask(SIG_SETMASK, &newset, &oldset) == -1) p_err("sigprocmask failed.");
	if ((pid = fork()) == -1) p_err("fork failed.");
	if (pid == 0){
		return 38;
	}else{
		int status;
		if (waitpid(pid, &status, 0) == -1) p_err("waitpid failed.");
		if (WIFEXITED(status)) printf("child exit status:%d.\n", WEXITSTATUS(status));
		else if (WIFSIGNALED(status)) printf("child killed by signal %d.\n", WTERMSIG(status));
		if (sigprocmask(SIG_SETMASK, &oldset, NULL) == -1) p_err("sigprocmask restore mask failed.");
		return 0;
	}
}
