#include "assist.h"

pthread_barrier_t barrier;

void *thread (void *arg)
{
	int ret;
	int seconds = (int)arg;
	printf("after %d seconds, i will reach barrier.\n", seconds);
	sleep(seconds);
	if ((ret = pthread_barrier_wait(&barrier)) == 0){
		printf("thread %d reached.\n", seconds);
	}else if (ret == PTHREAD_BARRIER_SERIAL_THREAD){
		printf("thread %d reached.\n", seconds);
		sleep(1);
		printf("all thread reached.\n");
	}else t_err("pthread_barrier_wait failed.", ret);
}

#define THREAD_NUM 10

int main (void)
{
	int ret;
	pthread_barrierattr_t battr;
	if (ret = pthread_barrierattr_init(&battr)) t_err("pthread_barrierattr_init failed.", ret);
	if (ret = pthread_barrierattr_setpshared(&battr, PTHREAD_PROCESS_PRIVATE)) t_err("pthread_barrierattr_setpshared failed.", ret);
	if (ret = pthread_barrier_init(&barrier, &battr, THREAD_NUM)) t_err("pthread_barrier_setpshared failed.", ret);
	if (ret = pthread_barrierattr_destroy(&battr)) t_err("pthread_barrier_destroy failed.", ret);

	pthread_attr_t tattr;
	if (ret = pthread_attr_init(&tattr)) t_err("pthread_mutexattr_init failed.", ret);
	if (pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED)) t_err("pthread_attr_setdetachstate failed.", ret);
	pthread_t tid;
	int i;
	for (i = 0; i < THREAD_NUM - 1; i++)
	if (ret = pthread_create(&tid, &tattr, thread, (void*)i)) t_err("pthread_create failed.", ret);
	if (ret = pthread_create(&tid, NULL, thread, (void*)i)) t_err("pthread_create failed.", ret);
	if (ret = pthread_join(tid, NULL)) t_err("pthread_join failed.", ret);

	return 0;
}
