#include "assist.h"

void *thread (void *arg)
{
	int ret;
	if (arg != (void*)0){
		sleep(2);
		pthread_t *tid;
		tid = (pthread_t *)arg;
		printf("in 3 seconds cancel another thread who's continually output.\n");
		sleep(3);
		if (ret = pthread_cancel(*tid)) fprintf(stderr, "pthread_cancel failed, errno:%d\n", ret);
		else printf("cancel issued.\n");
	}else{
		if (ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) fprintf(stderr, "pthread_setcancelstate failed.\n");
		else printf("set cancel state ok.\n");
		if (ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) fprintf(stderr, "pthread_setcanceltype failed.\n");
		else printf("set cancel type ok.\n");
		while (1){
			sleep(1);
			printf("in thread output.\n");
		}
	}
}

int main (void)
{
	pthread_t tid1, tid2;
	int ret;
	if (ret = pthread_create(&tid1, NULL, thread, (void*)0)) fprintf(stderr, "pthread_create failed, errno:%d.\n", ret);
	if (ret = pthread_create(&tid2, NULL, thread, (void*)&tid1)) fprintf(stderr, "pthread_create failed, errno:%d.\n", ret);
	if (ret = pthread_join(tid1, NULL)) fprintf(stderr, "pthread_join failed. errno:%d.\n", ret);
	if (ret = pthread_join(tid2, NULL)) fprintf(stderr, "pthread_join failed. errno:%d.\n", ret);
	return 0;
}
