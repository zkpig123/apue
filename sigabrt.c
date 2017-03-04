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
	act.sa_handler = sig_handler;
	if(sigaction(SIGABRT, &act, NULL) == -1) p_err("sigaction 4 SIGABRT failed.");
	sigset_t set;
	if (sigemptyset(&set)) err("sigemptyset.");
	if (sigaddset(&set, SIGABRT)) err("sigaddset.");
	if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) p_err("sigprocmask");
	printf("SIGABRT is blocked.\n");
	printf("kill self with SIGABRT.\n");
	kill(getpid(), SIGABRT);
	printf("is signal got? press any key to unblock it.\n");
	getchar();
	if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1) p_err("sigprocmask");
	printf("SIGABRT is unblocked.\n");

	return 0;
}
