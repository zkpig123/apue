//conclude: a process could receive signal from parent of
//from different session

#include "assist.h"

void sig_no (int signo)
{
	printf("signal %ld got.\n", signo);
}

int main (void)
{
	pid_t pid, sess_id, pgrp;
	if ((pid = fork()) == -1) p_err("fork 1 failed.");
	if (pid){ //parent
		if ((sess_id = getsid(0)) == (pid_t)-1) p_err("1st process getsid failed.");
		pgrp = getpgrp();
		printf("session id of 1st process is %ld, pgrp:%ld\n", sess_id, pgrp);
		/*sleep(5);
		printf("now kill 2nd process with sigcont.\n");
		if (kill(pid, SIGCONT) == -1) p_err("1st process kill 2nd process with sigcont failed.");*/
		if (waitpid(pid, NULL, 0) == -1) p_err("1st process wait failed.");
		return 0;
	}
	if ((pid = fork()) == -1) p_err("fork 2 failed.");
	if (pid == 0){
		if ((sess_id = getsid(0)) == (pid_t)-1) p_err("3nd process getsid failed.");
		pgrp = getpgrp();
		printf("session id of 3nd process is %ld, pgrp:%ld, now kill itself with SIGTSTP\n", sess_id, pgrp);
		kill(getpid(), SIGTSTP);
		printf("awakened.\n");
		return 0;
	}

	if (setsid() == (pid_t)-1) p_err("setsid failed.");
	if ((sess_id = getsid(0)) == (pid_t)-1) p_err("2nd process getsid failed.");
	pgrp = getpgrp();
	printf("session id of 2nd process is %ld, pgrp:%ld\n, now kill itself with sigtstp\n", sess_id, pgrp);
	sleep(3);
	if (kill(pid, SIGSTOP) == -1) p_err("2nd process kill 3rd process with sigcont failed.");
	printf("awakened.\n");
}
