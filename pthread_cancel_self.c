//default cancel state is enabled, default cancel type is deferred type that cancel is pending until reaching cancel point, we can use pthread_setcanceltype to change it to async cancel that cancel could happen anywhere.

#include "assist.h"

void *thread (void *arg)
{
	int ret;
	if (ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) fprintf(stderr, "pthread_setcancelstate failed.\n");
	else printf("setcancel state success.\n");
	if (ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) fprintf(stderr, "pthread_setcanceltype failed.\n");
	else printf("setcancel type success.\n");
	printf("now cancel self.\n");
	pthread_cancel(pthread_self());
	printf("shouldn't got here..\n");
}

int main (void)
{
	pthread_t tid;
	int ret;
	if (ret = pthread_create(&tid, NULL, thread, (void*)0)) fprintf(stderr, "pthread_create failed, errno:%d.\n", ret);
	if (ret = pthread_join(tid, NULL)) fprintf(stderr, "pthread_join failed. errno:%d.\n", ret);

	return 0;
}
