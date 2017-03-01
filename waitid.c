#include "assist.h"

int main (void)
{
	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork err.");
	if (pid == 0){
		//return 38; alternative test
		kill(getpid(), SIGTERM);
	}

	siginfo_t info;
	if(waitid(P_PID, (id_t)pid, &info, WEXITED) == -1) p_err("waitid failed.");
	printf("information from child:\npid:%ld, real uid:%ld, ", info.si_pid, info.si_uid);
	if (info.si_code == CLD_EXITED){
		printf("exit value:%d\n", info.si_status);
	}else{
		printf("killed by signal number:%d\n", info.si_status);
	}

	return 0;
}
