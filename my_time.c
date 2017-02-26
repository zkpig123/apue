#include "assist.h"

int main (int argc, char *argv[])
{
	if (argc == 1) err("usage: progname progname_to_run arg1 arg2...");
	long clock_ticks;
#ifdef CLK_TCK
	clock_ticks = CLK_TCK;
#else
	#ifdef _SC_CLK_TCK
	clock_ticks = sysconf(_SC_CLK_TCK);
	if (clock_ticks < 0) p_err("sysconf failed.");
	#else
	err("couldn't get clock ticks.");
	#endif
#endif
	char **arg_list;
	arg_list = &argv[2];
	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork err.");
	else if (pid == 0){ //child
		if (execv(argv[1], arg_list) == -1) p_err("execv failed.");
	}
	//parent
	struct tms buf;
	if (wait(NULL) == -1) p_err("wait child failed.");
	if (times(&buf) == (clock_t)-1) p_err("times func err.");
	printf("  real time:%.3f\n", 1.0 * (buf.tms_cutime + buf.tms_cstime) / clock_ticks);
	printf("  user time:%.3f\n", 1.0 * buf.tms_cutime);
	printf("system time:%.3f\n", 1.0 * buf.tms_cstime);

	return 0;
}
