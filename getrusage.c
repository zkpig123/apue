//conclude: ru_nsignals is not used on Linux 3.16.0?, ru_inblock,ru_oublock are not used on Linux 3.16.0?, ru_msgsnd, ru_msgrcv need test.

#include "assist.h"

void sig (int signo)
{
	return;
}

void print_usage (struct rusage *usage);

int main (void)
{
	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork failed.");
	else if (pid == 0){
		struct sigaction act;
		memset(&act, 0, sizeof(struct sigaction));
		act.sa_handler = sig;
		if (sigaction(SIGTERM, &act, NULL) == -1) p_err("child sigaction failed.");
		pid_t self_pid;
		self_pid = getpid();
		for (int i = 0; i < 3; i++) kill(self_pid, SIGTERM);
		char buf[10240];
		int fd;
		if ((fd = open("/dev/null", O_WRONLY)) == -1) p_err("child open failed.");
		for (int i = 0; i < 10240; i++) if (write(fd, buf, 1) != 1) p_err("child write failed.");
		return 0;
	}
	if (waitpid(pid, NULL, 0) == -1) p_err("waitpid failed.");
	struct rusage usage;
	if (getrusage(RUSAGE_CHILDREN, &usage) == -1) p_err("getrusage func failed.");
	printf("children usage:\n");
	print_usage(&usage);
	if (getrusage(RUSAGE_SELF, &usage) == -1) p_err("getrusage func failed.");
	printf("parent usage:\n");
	print_usage(&usage);
}

void print_usage (struct rusage *usage)
{
	printf("  user cpu time: %ld.%ld seconds\n", usage->ru_utime.tv_sec, usage->ru_utime.tv_usec);
	printf("system cpu time: %ld.%ld seconds\n", usage->ru_stime.tv_sec, usage->ru_stime.tv_usec);
	if (usage->ru_maxrss < 1024) printf(" maximum memory: %ld KB\n", usage->ru_maxrss);
	else if (usage->ru_maxrss < 1024 * 1024) printf(" maximum memory: %.3f MB\n", (double)usage->ru_maxrss / 1024);
	else printf("       maximum memory: %.3f GB\n", (double)usage->ru_maxrss / (1024 * 1024L));
	printf("integral shared memory size: %.3f KB\n", usage->ru_ixrss);
	printf("integral unshared data size: %.3f KB\n", usage->ru_idrss);
	printf("integral unshared stack size: %.3f KB\n", usage->ru_isrss);
	printf("page reclaims(soft page faults): %ld\n", usage->ru_minflt);
	printf("page faults(hard page faults): %ld\n", usage->ru_majflt);
	printf("swaps: %ld\n", usage->ru_nswap);
	printf("block input operations count: %ld\n", usage->ru_inblock);
	printf("block output operations count: %ld\n", usage->ru_oublock);
	printf("IPC messages sent: %ld\n", usage->ru_msgsnd);
	printf("IPC messages received: %ld\n", usage->ru_msgrcv);
	printf("signal count received: %ld\n", usage->ru_nsignals);
	printf("voluntary context switches:%ld\n", usage->ru_nvcsw);
	printf("involuntary context switches:%ld\n", usage->ru_nivcsw);
	printf("\n");
}
