#include "assist.h"

void sig_handler (int signo)
{
	printf("signal %ld catched.\n", signo);
}

int main (int argc, char *argv[])
{
	if (argc != 2) err("usage: progname 1|2.");
	if (strcmp("1", argv[1]) == 0){
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if(sigaction(SIGINT, &act, NULL) == -1) p_err("sigaction 4 SIGINT failed.");
	act.sa_handler = SIG_IGN;
	if(sigaction(SIGTSTP, &act, NULL) == -1) p_err("sigaction 4 SIGTSTP failed.");
	char c = 'A';
	int ret;
	while (((ret = read(STDIN_FILENO, &c, 1)) != -1 && c != 'E') || errno == EINTR){
		printf("in while. if want to exec, press 'E' and ENTER\n");
		sleep(1);
	}
	execl("./signal_exec", "signal_exec", "2", (char *)0);
	p_err("execl failed.");
		
	}else if (strcmp("2", argv[1]) == 0){
		char c = 'A';
		int ret;
		while (((ret = read(STDIN_FILENO, &c, 1)) != -1 && c != 'E') || errno != EINTR){
			printf("in while from exec. if want to exit, press 'E' and ENTER\n");
			sleep(1);
		}
	}else err("invalid 2nd parameter.");
}
