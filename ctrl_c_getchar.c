#include "assist.h"

void sig_handler (int signo)
{
	printf("signal %ld catched.\n", signo);
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if(sigaction(SIGINT, &act, NULL) == -1) p_err("sigaction 4 SIGINT failed.");

	char c = 100;
	errno = 0;
	if (read(STDIN_FILENO, &c, 1) != -1) printf("read get %d\n", c);
	else perror("read failed.");

	return 0;
}
