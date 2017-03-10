#include "assist.h"

int main (void)
{
#ifdef _POSIX_THREAD_SAFE_FUNCTIONS
	printf("_POSIX_THREAD_SAFE_FUNCTIONS is %d.\n", _POSIX_THREAD_SAFE_FUNCTIONS);
#elif defined (_SC_POSIX_THREAD_SAFE_FUNCTIONS)
	printf("_POSIX_THREAD_SAFE_FUNCTIONS(sysconf) is %ld\n", sysconf(_SC_POSIX_THREAD_SAFE_FUNCTIONS));
#else
	printf("_POSIX_THREAD_SAFE_FUNCTIONS not available.\n");

#endif
	return 0;
}
