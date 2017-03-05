//from apue: There are some interactions between the job-control signals. When any of the four stop signals (SIGTSTP, SIGSTOP, SIGTTIN, or SIGTTOU) is generated for a process, any pending SIGCONT signal for that process is discarded. Similarly, when the SIGCONT signal is generated for a process, any pending stop signals for that same process are discarded. just test it!

#include "assist.h"

void sig_handler (int signo)
{
	printf("signal %ld catched.\n", signo);
}

#include "assist.h"

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if(sigaction(SIGCONT, &act, NULL) == -1) p_err("sigaction 4 SIGCONT failed.");
	if(sigaction(SIGTSTP, &act, NULL) == -1) p_err("sigaction 4 SIGTSTP failed.");
	if(sigaction(SIGTTIN, &act, NULL) == -1) p_err("sigaction 4 SIGTTIN failed.");
	if(sigaction(SIGTTOU, &act, NULL) == -1) p_err("sigaction 4 SIGTTOU failed.");
	sigset_t newset;
	if (sigfillset(&newset)) err("sigfillset failed.");
	if (sigprocmask(SIG_SETMASK, &newset, NULL) == -1) p_err("sigprocmask failed.");
	pid_t pid = getpid();

	printf("all signal is blocked, now generate sigcont after generating sigtstp, sigttin, sigttou, then unblock all signals. these signals are catched. see the result.\n");
	kill(pid, SIGTSTP);
	kill(pid, SIGTTIN);
	kill(pid, SIGTTOU);
	kill(pid, SIGCONT);
	if (sigprocmask(SIG_UNBLOCK, &newset, NULL) == -1) p_err("sigprocmask failed.");
	printf("\n");

	printf("all signal is blocked, now generate sigtstp after generating sigcont, then unblock all signals. these signals are catched. see the result.\n");
	if (sigprocmask(SIG_SETMASK, &newset, NULL) == -1) p_err("sigprocmask failed.");
	kill(pid, SIGCONT);
	kill(pid, SIGTSTP);
	if (sigprocmask(SIG_UNBLOCK, &newset, NULL) == -1) p_err("sigprocmask failed.");
	printf("\n");

	printf("all signal except sigcont is blocked, now generate sigcont after generating sigtstp, sigttou, sigttin, and then unblock all signals. these signals are catched. see the result.\n");
	if (sigdelset(&newset, SIGCONT)) err("sigdelset failed.");
	if (sigprocmask(SIG_SETMASK, &newset, NULL) == -1) p_err("sigprocmask failed.");
	kill(pid, SIGTSTP);
	kill(pid, SIGTTIN);
	kill(pid, SIGTTOU);
	kill(pid, SIGCONT);
	if (sigprocmask(SIG_UNBLOCK, &newset, NULL) == -1) p_err("sigprocmask failed.");
	printf("\n");

	printf("all signal except sigttou is blocked, now generate sigttou after generating sigcont, and then unblock all signals. these signals are catched. see the result.\n");
	if (sigfillset(&newset)) err("sigfillset failed.");
	if (sigdelset(&newset, SIGTTOU)) err("sigdelset failed.");
	if (sigprocmask(SIG_SETMASK, &newset, NULL) == -1) p_err("sigprocmask failed.");
	kill(pid, SIGCONT);
	kill(pid, SIGTTOU);
	if (sigprocmask(SIG_UNBLOCK, &newset, NULL) == -1) p_err("sigprocmask failed.");
	printf("\n");
	
	return 0;
}
