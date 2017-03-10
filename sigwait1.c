//test if a signal is delivered to a thread 
//who is blockked at sigwait with a signal handler
//for that signal, whether signal handler is invoked
//or sigwait return without invoking signal handler.

//conclude: on linux, sigwait return without signal 
//handler invoking

#include "assist.h"

void sighandler (int signo)
{
	printf("in sighandler.\n");
}

void *thread (void *arg)
{
	sigset_t set;
	int ret;

	if (sigemptyset(&set)) err("sigemptyset failed.");
	if (sigaddset(&set, SIGQUIT)) err("sigemptyset failed.");
	int signo;
	printf("in thread, now sigwait.\n");
	if (sigwait(&set, &signo)) t_err("sigwait failed.", ret);
	else{
		printf("sigwait got signal %d\n", signo);
	}
}

int main (void)
{
	pthread_t tid;
	int ret;
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = sighandler;
	if (sigaction(SIGQUIT, &act, NULL) == -1) p_err("sigaction failed.");
	if (ret = pthread_create(&tid, NULL, thread, (void*)0)) t_err("pthread_create failed.", ret);
	sleep(2);
	printf("send signal to thread.\n");
	pthread_kill(tid, SIGQUIT);
	if (ret = pthread_join(tid, NULL)) t_err("pthread_join failed.", ret);

	return 0;
}
