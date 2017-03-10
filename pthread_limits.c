#include "assist.h"

int main (void)
{
#ifdef PTHREAD_DESTRUCTOR_ITERATIONS
	printf("PTHREAD_DESTRUCTOR_ITERATIONS(maximum number of times an implementation will try to destroy the thread-specific data when a thread exits): %ld\n", PTHREAD_DESTRUCTOR_ITERATIONS);
#else
	#ifdef _SC_THREAD_DESTRUCTOR_ITERATIONS
	printf("PTHREAD_DESTRUCTOR_ITERATIONS(maximum number of times an implementation will try to destroy the thread-specific data when a thread exits): %ld\n", (long int)sysconf(_SC_PTHREAD_DESTRUCTOR_ITERATIONS));
	#endif
#endif

#ifdef PTHREAD_KEYS_MAX
	printf("PTHREAD_KEYS_MAX(maximum number of keys that can be created by a process): %ld\n", PTHREAD_KEYS_MAX);
#else
	#ifdef _SC_THREAD_KEYS_MAX
	printf("PTHREAD_KEYS_MAX(maximum number of keys that can be created by a process): %ld\n", (long int)sysconf(_SC_PTHREAD_KEYS_MAX));
	#endif
#endif

#ifdef PTHREAD_STACK_MIN
	printf("PTHREAD_STACK_MIN(minimum number of bytes that can be used for a thread's stack): %ld\n", PTHREAD_STACK_MIN);
#else
	#ifdef _SC_THREAD_STACK_MIN
	printf("PTHREAD_STACK_MIN(minimum number of bytes that can be used for a thread's stack): %ld\n", (long int)sysconf(_SC_PTHREAD_STACK_MIN));
	#endif
#endif

#ifdef PTHREAD_THREADS_MAX
	printf("PTHREAD_THREADS_MAX(maximum number of threads that can be created in a process): %ld\n", PTHREAD_THREADS_MAX);
#else
	#ifdef _SC_THREAD_THREADS_MAX
	printf("PTHREAD_THREADS_MAX(maximum number of threads that can be created in a process): %ld\n", (long int)sysconf(_SC_THREAD_THREADS_MAX));
	#endif
#endif

	return 0;
}
