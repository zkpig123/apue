#include "assist.h"

int main (void)
{
	pid_t pid;
	if ((pid = fork()) == -1) p_err("first fork failed.");
	if (pid == 0){
		if ((pid = fork()) == -1) p_err("2nd fork failed.");
		if (pid) return 0;
		sleep(5);
		return 0;
	}else{
		int status;
		if (waitpid(pid, &status, 0) == -1) p_err("waitpid 1 failed.");
		else printf("waitpid 1st.\n");
		if (waitpid(pid, &status, 0) == -1) p_err("waitpid 2 failed.");
		else printf("waitpid 2nd.\n");
	}
}
