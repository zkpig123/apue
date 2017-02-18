//1.given pid of a process, obtain
//CPU-time clock of this process
//2.usage: progname pid

#include "assist.h"

int main (int argc, char *argv[])
{
	pid_t pid;
	if (argc == 1) pid = getpid();
	else if (argc == 2){
		if ((pid = atoi(argv[1])) < 0)
		err("invalid pid.");
	}
	else if (argc != 2) err("usage: progname pid.");
	clockid_t clk_id;
	int ret;
	if ((ret = clock_getcpuclockid(pid, &clk_id))){
		fprintf(stderr, "clock_getcpuclockid failed. errno:%d\n", ret);
		exit(1);
	}
	printf("CPU-time clock id of pid %ld is %d\n", (long)pid, (int)clk_id);

	return 0;
}
