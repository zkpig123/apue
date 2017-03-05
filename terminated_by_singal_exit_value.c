//exit number of child is meaningless(here 0)
//if child is killed by signal

#include "assist.h"

int main (void)
{
	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork failed.");
	if (pid){
		sleep(1);
		kill(pid, SIGKILL);
		int status;
		if (waitpid(pid, &status, 0) == -1) p_err("waitpid failed.");
		printf("exit status of child is %d\n", WEXITSTATUS(status));
		if (WIFSIGNALED(status)) printf("child is killed by %d\n", WTERMSIG(status));
		printf("value of status is %d\n", status);
		return 0;
	}else{
		while (1);
		return 3;
	}
}
