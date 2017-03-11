#include "assist.h"

int main (void)
{
	//change umask
	if (umask(0) == -1) p_err("umask failed.\n");
	
	pid_t pid;
	//fork one time, then setsid, then fork second time
	if ((pid = fork()) == -1) p_err("fork1 failed.\n");
	if (pid) return 0;
	if (setsid() == -1) p_err("setsid failed.\n");
	if ((pid = fork()) == -1) p_err("fork 2 failed.\n");
	if (pid) return 0;
	
	//change current working directory
	if (chdir("/") == -1) p_err("chdir to / failed.\n");
	
	int openmax;
	//close descriptor
#ifdef OPENMAX
	
	//redirect descriptor 0,1,2 to /dev/null
}
