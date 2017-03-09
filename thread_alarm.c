//from apue: all threads share the same set of alarms. 
///Thus, it is not possible for multiple 
//threads in a process to use alarm 
//timers without interfering (or cooperating) 
//with one another

//test: in 2 thread, each invoke alarm(2),see whether
//there's two output from signal handler of sigalrm, or
//just one output and another blocked

//conclude: apue is right

#include "assist.h"

jmp_buf jmp;

void sig_handler (int signo)
{
	printf("in sig handler, thread %ld\n", (long)pthread_self() % 10);
	longjmp(jmp, 1);
}

void *thread (void *arg)
{
	pthread_t tid;
	tid = pthread_self();
	printf("in thread %ld, now set a alarm of 2 seconds\n", (long)tid % 10);
	if (setjmp(jmp) == 0){
		alarm(2);
		pause();
	}else{
		printf("thread %d, longjmp got here.\n", (long)tid % 10);
	}
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if (sigaction(SIGALRM, &act, NULL) == -1) p_err("sigaction err.");

	pthread_t tid1, tid2;
	int ret;
	if (ret = pthread_create(&tid1, NULL, thread, (void*)0)) t_err("pthread_create err.", ret);
	if (ret = pthread_create(&tid2, NULL, thread, (void*)0)) t_err("pthread_create err.", ret);
	if (ret = pthread_join(tid1, NULL)) t_err("pthread_join err.", ret);
	if (ret = pthread_join(tid2, NULL)) t_err("pthread_join err.", ret);

	return 0;
}
