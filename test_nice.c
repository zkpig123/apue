//usage:test max and min of nice, and cpu usage
//with different nice value, get and set nice
//with getpriority and setpriority function.

#include "assist.h"

jmp_buf jmpbuf;

void sig_prof (int signo)
{
	dprintf(STDOUT_FILENO, "in sigaction, pid:%ld\n", (long)getpid());
	if (signo == SIGALRM) dprintf(STDOUT_FILENO, "SIGALRM.\n");
	else if (signo == SIGPROF) dprintf(STDOUT_FILENO, "SIGPROF.\n");
	else dprintf(STDOUT_FILENO, "SIGVTALRM.\n");
	longjmp(jmpbuf, 1);
}

int main (void)
{
	int nzero;
	int nice_value;
	int real_nice_value;
#ifdef NZERO
	nzero = NZERO;
#elif defined(_SC_NZERO)
	errno = 0;
	if ((nzero = sysconf(_SC_NZERO)) == -1 && errno) p_err("sysconf to get NZERO failed.");
#else
#error NZERO undefined
#endif
	printf("NZERO is %ld\n", nzero);
	if ((nice_value = nice(0)) == -1) p_err("nice func failed.");
	printf("current nice is %d\n", nice_value + nzero);
	printf("fork and set nice of child to ten more.\n");
	volatile pid_t pid;

	struct sigaction act;
	act.sa_sigaction = NULL;
	act.sa_flags = 0;
	act.sa_restorer = NULL;
	if (sigemptyset(&act.sa_mask)) err("sigemptyset failed.");
	act.sa_handler = sig_prof;
	if (sigaction(SIGALRM, &act, NULL) == -1){
		p_err("sigaction failed.");
	}

	sigset_t sig_set;
	if (sigemptyset(&sig_set)) err("sigemptyset failed.");
	if (sigprocmask(SIG_SETMASK, &sig_set, NULL) == -1) p_err("sigprocmask failed.");

	struct itimerval timer;
	if ((pid = fork()) == -1) p_err("fork failed.");
	//both parent and child
	if (pid == 0){
		errno = 0;
		nice(10);
		if (errno) p_err("child nice func failed.");
	}
	errno = 0;
	nice_value = getpriority(PRIO_PROCESS, 0);
	if (pid){
		if (errno) p_err("parent getpriority func failed.");
		printf("nice value of parent is %d\n", nice_value + nzero);
		if (waitpid(pid, NULL, 0) == -1) p_err("waitpid failed.");
	}else{
		if (errno) p_err("child getpriority func failed.");
		printf("nice value of child is %d\n", nice_value + nzero);
	}
	timer.it_value.tv_sec = 5;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	volatile long long count;
	if (setjmp(jmpbuf) == 0){
		if (setitimer(ITIMER_REAL, &timer, NULL) == -1) p_err("setitimer failed.");
		for (count = 0; ; ) count++;
	}else{
		printf("after longjmp.\n");
		if (pid) printf("parent count:%lld\n", count);
		else printf("child count:%lld\n", count);
	}

	return 0;
}
