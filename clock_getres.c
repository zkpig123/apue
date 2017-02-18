#include "assist.h"

#define NANO_SECOND_BITS 9

int get_bits (long nanosecond)
{
	int bits;
	for (bits = 0; nanosecond > 0; nanosecond /= 10, bits++)
		;
	return bits;
}

int print_res (clockid_t clk_id)
{
	struct timespec res;
	int zero_bits;
	switch (clk_id){
	case CLOCK_REALTIME:
		if (clock_getres(CLOCK_REALTIME, &res) == -1) perror("clock_getres of CLOCK_REALTIME failed.");
		printf("CLOCK_REALTIME resolution:%d.", (int)res.tv_sec);
		break;
	case CLOCK_MONOTONIC:
		if (clock_getres(CLOCK_MONOTONIC, &res) == -1) perror("clock_getres of CLOCK_REALTIME failed.");
		printf("CLOCK_MONOTONIC resolution:%d.", (int)res.tv_sec);
		break;
	case CLOCK_PROCESS_CPUTIME_ID:
		if (clock_getres(CLOCK_PROCESS_CPUTIME_ID, &res) == -1) perror("clock_getres of CLOCK_REALTIME failed.");
		printf("CLOCK_PROCESS_CPUTIME_ID resolution:%d.", (int)res.tv_sec);
		break;
	case CLOCK_THREAD_CPUTIME_ID:
		if (clock_getres(CLOCK_THREAD_CPUTIME_ID, &res) == -1) perror("clock_getres of CLOCK_REALTIME failed.");
		printf("CLOCK_THREAD_CPUTIME_ID resolution:%d.", (int)res.tv_sec);
		break;
	default:
		fprintf(stderr, "wrong clock_id.\n");
		return 1;
	}
	zero_bits = NANO_SECOND_BITS - get_bits(res.tv_nsec);
	for (int i = 0; i < zero_bits; i++) printf("0");
	printf("%ld\n", res.tv_nsec);
	return 0;
}

int main (void)
{
	print_res(CLOCK_REALTIME);
	print_res(CLOCK_MONOTONIC);
	print_res(CLOCK_PROCESS_CPUTIME_ID);
	print_res(CLOCK_THREAD_CPUTIME_ID);

	return 0;
}
