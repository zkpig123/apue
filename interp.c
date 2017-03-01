#include "assist.h"

int main (void)
{
	pid_t pid;
	if ((pid = fork()) < 0){
		p_err("fork failed..");
	}else if (pid == 0){
		if (execl("./interp.sh", "arg1", "arg2", (char *)0) == -1) p_err("execl failed.");
	}
	if (waitpid(pid, NULL, 0) == -1) p_err("waitpid failed.");

	return 0;
}
