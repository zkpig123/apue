#include "assist.h"

void sig_handler (int signo)
{
	printf("signal %ld catched.\n", signo);
	exit(0);
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	//if(sigaction(SIGCONT, &act, NULL) == -1) p_err("sigaction 4 SIGCHLD failed."); //whether run this sentense differed!
	sigset_t set;
	if (sigemptyset(&set)) err("sigemptyset failed.");
	sigsuspend(&set);
	printf("after sigsuspend.\n");
}
