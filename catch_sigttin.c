//test method: run program in background

//conclude: SIGTTIN couldn't be catched? no,
//if we don't catch the signal SIGTTIN, then 
//program will be stopped(like ctrl+z), but
//if we did catch the signal SIGTTIN, then
//the read operation just return error with
//errno set to EINTR, stand input also return
//error which could be determinted by using
//ferror

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
	if (sigaction(SIGTTIN, &act, NULL) == -1) p_err("sigaction 4 SIGTTIN failed.");

	while (getchar()){
		if(ferror(stdin)) printf("getchar failed.");
		clearerr(stdin);
	}

	return 0;
}
