#include "assist.h"

void *thread (void *arg)
{
	int ret;
	int sig = (int)(long)arg;
	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask, sig);
	if (ret = pthread_sigmask(SIG_SETMASK, &mask, NULL)) p_err("pthread_sigmask failed.");
	printf("thread %d, sig : %d.\n", sig, sig);
	while (1){
	}
}

void sig_handler (int signo, siginfo_t *info, void *arg)
{
	dprintf(STDOUT_FILENO, "thread %d : in sighandler, signo:%d.\n", signo, signo);
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = sig_handler;
	if (sigaction(SIGRTMIN, &act, NULL) == -1) p_err("sigaction failed.");
	if (sigaction(SIGRTMAX, &act, NULL) == -1) p_err("sigaction failed.");
	printf("signo SIGRTMIN : %d, signo SIGRTMAX : %d.\n", SIGRTMIN, SIGRTMAX);

	sigset_t mask;
	sigfillset(&mask);
	int ret;
	if (ret = pthread_sigmask(SIG_SETMASK, &mask, NULL)) p_err("pthread_sigmask failed.");
	pthread_t tid1, tid2;
	if (ret = pthread_create(&tid1, NULL, thread, (void*)SIGRTMIN)) t_err("pthread_create failed.", ret);
	if (ret = pthread_create(&tid2, NULL, thread, (void*)SIGRTMAX)) t_err("pthread_create failed.", ret);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}
