#include "assist.h"

struct sigaction act;

void sighandler_main (int signo)
{
	dprintf(STDOUT_FILENO, "in sighandler installed in main.\n");
}

void sighandler_thread (int signo)
{
	dprintf(STDOUT_FILENO, "in sighandler changed in thread.\n");
}

void *thread (void *arg)
{
	int ret;
	int id = (int)arg;
	if ((int)arg != 0){
		sleep(2);
		act.sa_handler = sighandler_thread;
		if (sigaction(SIGQUIT, &act, NULL) == -1) p_err("sigaction failed.");
	}
	else{
		if (ret = pthread_kill(pthread_self(), SIGQUIT)) t_err("pthread_kill failed.", ret);
		sleep(3);
		if (ret = pthread_kill(pthread_self(), SIGQUIT)) t_err("pthread_kill failed.", ret);
	}
}

int main (void)
{
	pthread_t tid1, tid2;
	int ret;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sighandler_main;
	if (sigaction(SIGQUIT, &act, NULL) == -1) p_err("sigaction failed.");
	if (ret = pthread_create(&tid1, NULL, thread, (void*)&tid2)) t_err("pthread_create err.", ret);
	if (ret = pthread_create(&tid2, NULL, thread, (void*)0)) t_err("pthread_create err.", ret);
	if (ret = pthread_join(tid1, NULL)) t_err("pthread_join failed.", ret);
	if (ret = pthread_join(tid2, NULL)) t_err("pthread_join failed.", ret);

	return 0;
}
