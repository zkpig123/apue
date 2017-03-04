//conclude: linux has no SIGINFO, generally SIGINFO
//is generated using ctrl+t

#include "assist.h"

#define SIGINFO SIGPWR

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
	if(sigaction(SIGINFO, &act, NULL) == -1) p_err("sigaction 4 SIGINFO failed.");
	while (1){
		printf("process:%ld.\n", getpid());
		sleep(1);
	}

	return 0;
}
