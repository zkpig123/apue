#include "assist.h"

int main (void)
{
	pthread_spinlock_t lock;
	int ret;
	if (ret = pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE)){
		fprintf(stderr, "spin init failed, errno:%d\n", ret);
		return 1;
	}
	if (ret = pthread_spin_lock(&lock)){
		fprintf(stderr, "spin lock 1 failed, errno:%d\n", ret);
		return 2;
	}
	if (ret = pthread_spin_lock(&lock)){
		fprintf(stderr, "spin lock 2 failed, errno:%d\n", ret);
		return 3;
	}

	return 0;
}
