//conclude: no matter whether we catch 
//SIGALRM or whether we call longjmp from
//signal handler, even if we block all signals
//from signal handler, abort always abort the
//program!

#include "assist.h" jmp_buf jmp;

void sig_handler (int signo)
{
	sigset_t set;
	printf("signal %ld catched.\n", signo);
	if (sigfillset(&set)) err("sigfillset.");
	if (sigprocmask(SIG_SETMASK, &set, NULL) == -1) p_err("sigprocmask failed.");
	longjmp(jmp, 1);
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if(sigaction(SIGABRT, &act, NULL) == -1) p_err("sigaction 4 SIGABRT failed.");
	if (setjmp(jmp) == 0) abort();
	else printf("after abort.\n");
}
