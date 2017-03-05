#include "assist.h"

//extern char *sys_siglist[];

int main (void)
{
	for (int i = 1; i <= SIGRTMAX; i++){
		printf("using sys_siglist array pointer to output signal description.\n");
		printf("signal %d is %s\n", i, sys_siglist[i]);
		fflush(stdout);
		printf("----\n");

		printf("using psignal function to output signal description to stderr.\n");
		fflush(stdout);
		psignal(i, NULL);
		printf("----\n");

		printf("using psignal function to output signal description.\n");
		printf("%s\n", strsignal(i));
		printf("\n");
		fflush(stdout);
	}

	return 0;
}
