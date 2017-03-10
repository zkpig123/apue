//test reentrant way to use stand library input/ouput in multi-threaded env

#include "assist.h"

void *thread (void *arg)
{
	flockfile(stdout);
	printf("thread %d :get FILE stream lock.\n", (int)arg);
	int i = 0;
	while (i++ < 4){
		printf("thread %d output.\n", (int)arg);
		sleep(1);
	}
	funlockfile(stdout);
	printf("thread %d :release FILE stream lock.\n");
}

int main (void)
{
	pthread_t tid1, tid2;
	int ret;
	if (ret = pthread_create(&tid1, NULL, thread, (void*)1)) t_err("pthread_create failed.", ret);
	if (ret = pthread_create(&tid2, NULL, thread, (void*)2)) t_err("pthread_create failed.", ret);
	if (ret = pthread_join(tid1, NULL)) t_err("pthread_join failed.", ret);
	if (ret = pthread_join(tid2, NULL)) t_err("pthread_join failed.", ret);

	return 0;
}
