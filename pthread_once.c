//test whether pthread_once use mutex and condition variable 
//mechanism: we call pthread_once in thread, and pause or make
//the thread sleep, and see whether there's output from other
//thread.

#include "assist.h"

pthread_once_t once;
int global;

void once_func (void)
{
	global++;
	sleep(5);
}

void *thread (void *arg)
{
	int status;
	int value;
	value = (int)arg;
	status = pthread_once(&once, once_func);
	printf("thread %d: after once, status:%d\n", value, status);
}

int main (void)
{
	pthread_t thread_id1, thread_id2, thread_id3;
	pthread_create(&thread_id1, NULL, thread, (void*)1);
	pthread_create(&thread_id2, NULL, thread, (void*)2);
	pthread_create(&thread_id3, NULL, thread, (void*)3);
	pthread_join(thread_id1, NULL);
	pthread_join(thread_id2, NULL);
	pthread_join(thread_id3, NULL);
	printf("initially global is:0, now global is:%d\n", global);

	return 0;
}
