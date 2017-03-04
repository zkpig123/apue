#include "assist.h"

void sig_handler (int signo)
{
	printf("signal %ld catched.\n", signo);
	exit(0);
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if(sigaction(SIGPIPE, &act, NULL) == -1) p_err("sigaction 4 SIGPIPE failed.");

	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) p_err("socket func failed.");
	if (send(sockfd, "hi", 2, 0) == -1) p_err("send func failed.");

	return 0;
}
