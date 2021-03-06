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
	if(sigaction(SIGXCPU, &act, NULL) == -1) p_err("sigaction 4 SIGXCPU failed.");
	struct rlimit rlim;
	if (getrlimit(RLIMIT_CPU, &rlim) == -1) p_err("getrlimit failed.");
	printf("RLIMIT_CPU: soft:%ld, hard:%ld\n", rlim.rlim_cur, rlim.rlim_max);
	printf("set RLIMIT_CPU soft limit to 5(RLIMIT_CPU unit is seconds).\n");
	rlim.rlim_cur = 5;
	if (setrlimit(RLIMIT_CPU, &rlim) == -1) p_err("setrlimit failed.");
	printf("now RLIMIT_CPU: soft:%ld, hard:%ld\n", rlim.rlim_cur, rlim.rlim_max);
	while (1);

	return 0;
}
