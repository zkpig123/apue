#include "assist.h"

#define NANO_SECOND_BITS 9

int get_bits (long nanosecond)
{
	int bits;
	for (bits = 0; nanosecond > 0; nanosecond /= 10, bits++)
		;
	return bits;
}

int print_time (clockid_t clk_id)
{
	struct timespec tp;
	int zero_bits;
	switch (clk_id){
	case CLOCK_REALTIME:
		if (clock_gettime(CLOCK_REALTIME, &tp) == -1) perror("clock_gettime of CLOCK_REALTIME failed.");
		printf("          CLOCK_REALTIME is now: :%d.", (int)tp.tv_sec);
		break;
	case CLOCK_MONOTONIC:
		if (clock_gettime(CLOCK_MONOTONIC, &tp) == -1) perror("clock_gettime of CLOCK_REALTIME failed.");
		printf("         CLOCK_MONOTONIC is now: :%d.", (int)tp.tv_sec);
		break;
	case CLOCK_PROCESS_CPUTIME_ID:
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp) == -1) perror("clock_gettime of CLOCK_REALTIME failed.");
		printf("CLOCK_PROCESS_CPUTIME_ID is now: :%d.", (int)tp.tv_sec);
		break;
	case CLOCK_THREAD_CPUTIME_ID:
		if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp) == -1) perror("clock_gettime of CLOCK_REALTIME failed.");
		printf(" CLOCK_THREAD_CPUTIME_ID is now: :%d.", (int)tp.tv_sec);
		break;
	default:
		fprintf(stderr, "wrong clock_id.\n");
		return 1;
	}
	zero_bits = NANO_SECOND_BITS - get_bits(tp.tv_nsec);
	for (int i = 0; i < zero_bits; i++) printf("0");
	printf("%ld\n", tp.tv_nsec);
	return 0;
}

int main (void)
{
	print_time(CLOCK_REALTIME);
	print_time(CLOCK_MONOTONIC);
	print_time(CLOCK_PROCESS_CPUTIME_ID);
	print_time(CLOCK_THREAD_CPUTIME_ID);

	return 0;
}
