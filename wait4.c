#include "assist.h"

void *thread (void *arg)
{
	return (void *)5;
}

int main(void)
{
	pid_t pid;
	if ((pid = fork()) == 0){ //child
		pthread_t p_id;
		if (pthread_create(&p_id, NULL, thread, (void*)0)) p_err("pthread_create failed.");
		sleep(2);
		return 0;
	}else if (pid == -1) p_err("fork failed."); //err
	//parent
	int status, options;
	struct rusage res;
	options = 0;
	if (wait4(pid, &status, options, &res) == -1) p_err("wait4 failed.");
	if (WIFEXITED(status)) printf("exit code:%d\n", WEXITSTATUS(status));
	else if (WIFSIGNALED(status)) printf("exit signal:%d\n", WTERMSIG(status));
	else if (WSTOPSIG(status)) printf("child stopped.\n");
	else printf("child continue.\n");
	printf("user cpu time of child:%d.%03ds.\n", res.ru_utime.tv_sec, res.ru_utime.tv_usec);
	printf("system cpu time of child:%d.%03ds.\n", res.ru_stime.tv_sec, res.ru_stime.tv_usec);
	printf("maximum resident set size of child:%ldKB\n", res.ru_maxrss);
	printf("page reclaims (soft page faults):%ld\n", res.ru_minflt);
	printf("page faults (hard page faults):%ld\n", res.ru_majflt);
	printf("block input operation:%ld\n", res.ru_inblock);
	printf("block output operation:%ld\n", res.ru_oublock);
	printf("block output operation:%ld\n", res.ru_oublock);
	printf("voluntary context switches:%ld\n", res.ru_nvcsw);
	printf("involuntary context switches:%ld\n", res.ru_nivcsw);

	return 0;
}
