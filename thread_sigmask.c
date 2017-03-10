//test: sigmask of thread is inherited from calling thread

#include "assist.h"

struct sigaction act;

void sig_handler(int signo)
{
	dprintf(STDOUT_FILENO, "in sighandler.\n");
}

void *thread_thread (void *arg)
{
	int ret;
	printf("in thread create from thread, now kill self with SIGQUIT.\n");
	if (ret = pthread_kill(pthread_self(), SIGQUIT)) t_err("pthread_kill failed.", ret);
}

void *thread (void *arg)
{
	int ret;
	int id = (int)arg;
	printf("in thread create from main, now kill self with SIGQUIT.\n");
	if (ret = pthread_kill(pthread_self(), SIGQUIT)) t_err("pthread_kill failed.", ret);

	sigset_t mask;
	if (sigemptyset(&mask)) err("sigemptyset failed.");
	if (sigaddset(&mask, SIGQUIT)) err("sigaddset failed.");
	if (ret = pthread_sigmask(SIG_BLOCK, &mask, NULL)) t_err("pthread_sigmask failed.", ret);
	pthread_t tid;
	if (ret = pthread_create(&tid, NULL, thread_thread, (void*)0)) t_err("pthread_create failed.", ret);
	if (ret = pthread_join(tid, NULL)) t_err("pthread_join failed in thread.\n", ret);
}

int main (void)
{
	pthread_t tid;
	int ret;

	printf("in main, set handler of sigquit.\n");
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if (sigaction(SIGQUIT, &act, NULL) == -1) p_err("sigaction failed.");
	if (ret = pthread_create(&tid, NULL, thread, (void*)0)) t_err("pthread_create err.", ret);
	if (ret = pthread_join(tid, NULL)) t_err("pthread_join failed.", ret);

	return 0;
}
