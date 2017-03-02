//test whether a process can set process 
//group id of itself to an id not equal to
//pid of any process currently running.

//conclude:unable
#include "assist.h"

int main (void)
{
	pid_t pgid = 2200;
	if (setpgid(0, pgid) == -1) p_err("setpgid failed.");
	printf("setpgid success. current pgid is %ld\n", getpgrp());

	return 0;
}
