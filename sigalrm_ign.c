#include "assist.h"

void sig_handler (int signo)
{
	printf("signal %ld catched.\n", signo);
	exit(0);
}

int main(void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	if(sigaction(SIGALRM, &act, NULL) == -1) p_err("sigaction 4 SIGALRM failed.");
	alarm(1);
	sleep(3);
	act.sa_handler = SIG_DFL;
	if(sigaction(SIGALRM, &act, NULL) == -1) p_err("sigaction 4 SIGALRM failed.");
	sleep(1);
	printf("exit.\n");

	return 0;
}
