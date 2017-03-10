#include "assist.h"

pthread_key_t key;
pthread_once_t once;

void key_destructor (void *arg)
{
}

void init_key (void)
{
	int ret;
	if (key = pthread_key_create(&key, key_destructor)) t_err("pthread_key_create failed.", ret);
}

int main (void)
{
	int ret;
	void *value;

	if (ret = pthread_once(&once, init_key)) t_err("pthread_once failed.", ret);
	value = pthread_getspecific(key);
	printf("key is %p\n", value);
	if (ret = pthread_setspecific(key, (void*)38)) t_err("pthread_setspecific failed.", ret);
	else{
		value = pthread_getspecific(key);
		printf("key is %p\n", value);
	}

	return 0;
}
