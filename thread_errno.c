#include "assist.h"

void *thread (void *arg)
{
	printf("thread: original errno = %d\n", errno);
	while (1){
		printf("thread: errno is %d\n", errno);
		sleep(1);
	}
}

int main (void)
{
	pthread_t tid;
	int ret;

	if (ret = pthread_create(&tid, NULL, thread, (void*)0)) t_err("pthread_create failed.", ret);
	while (1){
		sleep(1);
		printf("set errno to 38.\n");
	}
	if (ret = pthread_join(tid, NULL)) t_err("pthread_join failed.", ret);
}
