//test method: 1.run program and type CTRL+C, CTRL+Z, CTRL+\
//             2.run program in background
//conclude: method 2, strange, catch sigquit, printf in 
//signal handler and printf after returning from signal handler 
//cause signal to generate over and over again, but output
//appear in the terminal!

#include "assist.h"

void sig_handler (int signo)
{
	printf("signal catched, signal number:%d\n", signo); 
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if (sigaction(SIGTSTP, &act, NULL) == -1) p_err("sigaction 4 SIGTSTP failed.");
	if (sigaction(SIGINT, &act, NULL) == -1) p_err("sigaction 4 SIGINT failed.");
	if (sigaction(SIGQUIT, &act, NULL) == -1) p_err("sigaction 4 SIGQUIT failed.");
	if (sigaction(SIGTTOU, &act, NULL) == -1) p_err("sigaction 4 SIGTTOU failed.");
	while (1){
		sleep(1);
		printf("output.\n");
	}

	return 0;
}
