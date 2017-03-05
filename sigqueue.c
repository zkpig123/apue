//1.need work with test_sigqueue.c to send
//signal to.
//2.conclude: reliably queued signal mean if signal generate x
//times, then it will be delivered x times, unless SIGQUEUE_MAX 
//reached, but the order of delivery of signal is from minor number 
//to major number(to be confirmed!), eg, from 1 to 10, but if we
//don't block other signal in signal handler, then signal handler of
//earlier arrived will be interrupted by later arrived signal, so
//input from earlier arrived signal handler is later than that of 
//later arrived one, namely, in reverse order. eg. when unbloked,
//signal 1 is delivered earlier, 10 is later, but output from handler 
//of signal 10 will be earlier than signal 1, mislead us to think that 
//signal 10 is delivered preceding signal 1.
//3.usage: "./progname > outputfile", then using ps aux to obtain pid of
//process, and then invoke "test_sigqueue pid_of_current_process"

#include "assist.h"

void print_sigmask (sigset_t *set)
{
	if (set == NULL){
		printf("sigmask arg null.\n");
		return;
	}
	printf("sigmask:");
	if (sigismember(set, SIGHUP)) printf("SIGHUP ");
	if (sigismember(set, SIGINT)) printf("SIGINT ");
	if (sigismember(set, SIGQUIT)) printf("SIGQUIT ");
	if (sigismember(set, SIGILL)) printf("SIGILL ");
	if (sigismember(set, SIGTRAP)) printf("SIGTRAP ");
	if (sigismember(set, SIGABRT)) printf("SIGABRT ");
	if (sigismember(set, SIGBUS)) printf("SIGBUS ");
	if (sigismember(set, SIGFPE)) printf("SIGFPE ");
	if (sigismember(set, SIGKILL)) printf("SIGKILL ");
	if (sigismember(set, SIGUSR1)) printf("SIGUSR1 ");
	if (sigismember(set, SIGSEGV)) printf("SIGSEGV ");
	if (sigismember(set, SIGUSR2)) printf("SIGUSR2 ");
	if (sigismember(set, SIGPIPE)) printf("SIGPIPE ");
	if (sigismember(set, SIGALRM)) printf("SIGALRM ");
	if (sigismember(set, SIGTERM)) printf("SIGTERM ");
	if (sigismember(set, SIGCHLD)) printf("SIGCHLD ");
	if (sigismember(set, SIGCONT)) printf("SIGCONT ");
	if (sigismember(set, SIGSTOP)) printf("SIGSTOP ");
	if (sigismember(set, SIGTSTP)) printf("SIGTSTP ");
	if (sigismember(set, SIGTTIN)) printf("SIGTTIN ");
	if (sigismember(set, SIGTTOU)) printf("SIGTTOU ");
	if (sigismember(set, SIGURG)) printf("SIGURG ");
	if (sigismember(set, SIGXCPU)) printf("SIGXCPU ");
	if (sigismember(set, SIGXFSZ)) printf("SIGXFSZ ");
	if (sigismember(set, SIGVTALRM)) printf("SIGVTALRM ");
	if (sigismember(set, SIGPROF)) printf("SIGPROF ");
	if (sigismember(set, SIGWINCH)) printf("SIGWINCH ");
	if (sigismember(set, SIGPOLL)) printf("SIGPOLL ");
	if (sigismember(set, SIGSYS)) printf("SIGSYS ");
	for (int signo = SIGRTMIN; signo <= SIGRTMAX; signo++){
		if (sigismember(set, signo)) printf("%d ", signo);
	}
	printf("\n");
}

void print_context (ucontext_t *cont)
{
	printf("ucontext sturecure:\n");
	printf("uc_link:%p\n", cont->uc_link);
	print_sigmask(&cont->uc_sigmask);
	printf("uc_uc_stack:\n");
	printf("             base pointer:%p\n", cont->uc_stack.ss_sp);
	printf("             stack size:%zu\n", cont->uc_stack.ss_size);
	printf("             flags:%d\n", cont->uc_stack.ss_flags);
}

void print_siginfo (siginfo_t *info)
{
	printf("siginfo structure:\n");
	//psiginfo(info, "hi"); //alternatively using function from system to print siginfo struct
	printf("si_signo:%d\n", info->si_signo);
	printf("si_errno:%d\n", info->si_errno);
	switch (info->si_code){
	case SI_USER:
		printf("si_code: SI_USER\n");
		break;
	case SI_QUEUE:
		printf("si_code: SI_QUEUE\n");
		break;
	case SI_TIMER:
		printf("si_code: SI_TIMER\n");
		break;
	case SI_ASYNCIO:
		printf("si_code: SI_ASYNCIO\n");
		break;
	case SI_MESGQ:
		printf("si_code: SI_MESGQ\n");
		break;
	default:
		printf("si_code: %d\n", info->si_code);
		break;
	}
	printf("si_pid:%ld\n", (long)info->si_pid);
	printf("si_uid:%ld\n", (long)info->si_uid);
	switch (info->si_signo){
	case SIGBUS:
	case SIGILL:
	case SIGFPE:
	case SIGSEGV:
		printf("si_addr:%s\n", info->si_addr);
		break;
	}
	printf("si_status:%d\n", info->si_status);
	printf("si_value.sival_int:%d\n", info->si_value.sival_int);
}

void sig_action (int signo, siginfo_t *info, void *context)
{
	if (signo == SIGCONT){
		printf("sigcont. now should unblock all signal\n");
		print_siginfo(info);
		printf("------------\n");
		print_context((ucontext_t*)context);
		printf("\n");
	}
	else{
		printf("signal %ld catched.\n", signo);
		print_siginfo(info);
		printf("------------\n");
		print_context((ucontext_t*)context);
		printf("\n");
	}
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));

	//////
	/*here important! if we don't block signal while process signal, then
	 * output from signal handler will be in reverse order of delivery of
	 * signal, because signal handler of precedingly delivered signal will 
	 * be interrupted by signal delivered after
	 */
	if(sigfillset(&act.sa_mask)) err("sigfillset failed.");
	for (int signo = SIGRTMIN; signo <= SIGRTMAX; signo++){
		if (sigaddset(&act.sa_mask, signo)) err("sigaddset failed.");
		if (sigaction(signo, &act, NULL) == -1) p_err("sigaction 4 signal number failed.");
	}
	//////

	act.sa_sigaction = sig_action;
	act.sa_flags = SA_SIGINFO;
	if (sigaction(SIGCONT, &act, NULL) == -1) p_err("sigaction 4 SIGCONT failed.");
	if (sigaction(SIGABRT, &act, NULL) == -1) p_err("sigaction 4 SIGABRT failed.");
	if (sigaction(SIGCHLD, &act, NULL) == -1) p_err("sigaction 4 SIGABRT failed.");
	if (sigaction(SIGINT, &act, NULL) == -1) p_err("sigaction 4 SIGINT failed.");
	if (sigaction(SIGPOLL, &act, NULL) == -1) p_err("sigaction 4 SIGABRT failed.");
	sigset_t set;
	if (sigfillset(&set)) err("sigfillset failed.");
	for (int signo = SIGRTMIN; signo <= SIGRTMAX; signo++){
		if (sigaddset(&set, signo)) err("sigaddset failed.");
		if (sigaction(signo, &act, NULL) == -1) p_err("sigaction 4 signal number failed.");
	}
	if (sigdelset(&set, SIGCONT)) err("sigdelset failed.");
	printf("send signal including realtime signal but excluding SIGKILL SIGSTOP SIGTSTP and other hardware err signal.\n");
	errno = 38;
	sigsuspend(&set);
	//send normal signal and realtime signal more than one time to see whether queued
	
	return 0;
}
