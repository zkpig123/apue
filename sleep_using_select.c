#include "assist.h"

unsigned int sleep(unsigned int seconds)
{
	struct timeval tv, vt;
	time_t start, end;

	if (seconds == 0) return 0;
	tv.tv_sec = seconds;
	tv.tv_usec = 0;
	vt.tv_sec = seconds;
	vt.tv_usec = 0;
	start = time(NULL);
	if (select(0, NULL, NULL, NULL, &vt) == 0) return 0;
	end = time(NULL);
	long elapsed = end - start;
	printf("elapsed:%ld, tv_sec:%ld\n", elapsed, tv.tv_sec);
	if (elapsed >= tv.tv_sec) return tv.tv_sec;
	else return elapsed;
}

int main (void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	printf("now is %ld.%.3f seconds.\n", ts.tv_sec, ts.tv_nsec / 1000000000.0);
	printf("sleep %d seconds.\n", sleep(5));
	clock_gettime(CLOCK_MONOTONIC, &ts);
	printf("and now is %ld.%.3f seconds.\n", ts.tv_sec, ts.tv_nsec / 1000000000.0);

	return 0;
}
