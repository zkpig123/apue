#include <stdio.h>
#include <limits.h>
#include <inttypes.h>
#include <stdint.h>
#include "vm.c"

int main(void)
{
	short i = SHRT_MAX - 123;
	printf ("i = SHRT_MAX - 5839\n");
	printf ("%%hd %hd\n", i);
	printf ("%%d %d\n", i);
	printf ("%%hhd %hhd\n", i);
	vm(&i, 2);
	int64_t t = INT64_MAX;
	printf ("t = INT64_MAX\n");
	printf ("%%PRId64 %" PRId64 "\n", t);
	printf ("%%d %d\n", t);
	vm(&t, 8);
}
