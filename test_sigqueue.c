//using accompaning with sigqueue.c, see usage in sigqueue.c
//usage: progname pid_of_sigqueue(.c)

#include "assist.h"

int main (int argc, char *argv[])
{
	if (argc != 2) err("usage: progname pid_to_send_signal.");
	printf("pid of current process is %ld\n", (long)getpid());
	pid_t pid = atoi(argv[1]);
	union sigval val;

	val.sival_int = 38;
	sigqueue(pid, SIGRTMAX, val);
	printf("SIGRTMAX :      %d\n", SIGRTMAX);

	val.sival_int = 3838;
	sigqueue(pid, SIGRTMIN + 10, val);
	printf("SIGRTMIN + 10 : %d\n", SIGRTMIN + 10);

	val.sival_int = 382;
	sigqueue(pid, SIGRTMIN, val);
	printf("SIGRTMIN :      %d\n", SIGRTMIN);
	
	val.sival_int = 3838;
	sigqueue(pid, SIGRTMAX - 5, val);
	printf("SIGRTMAX - 5 :  %d\n", SIGRTMAX - 5);

	val.sival_int = 381;
	sigqueue(pid, SIGPOLL, val);
	printf("SIGPOLL :       %d\n", SIGPOLL);

	val.sival_int = 383;
	sigqueue(pid, SIGINT, val);
	printf("SIGINT :        %d\n", SIGINT);

	val.sival_int = 381;
	sigqueue(pid, SIGPOLL, val);
	printf("SIGPOLL :       %d\n", SIGPOLL);

	val.sival_int = 381;
	sigqueue(pid, SIGABRT, val);
	printf("SIGABRT :       %d\n", SIGABRT);

	val.sival_int = 381;
	sigqueue(pid, SIGCHLD, val);
	printf("SIGCHLD :       %d\n", SIGCHLD);

	val.sival_int = 3838;
	sigqueue(pid, SIGRTMIN + 10, val);
	printf("SIGRTMIN + 10 : %d\n", SIGRTMIN + 10);

	val.sival_int = 381;
	sigqueue(pid, SIGCHLD, val);
	printf("SIGCHLD :       %d\n", SIGCHLD);

	val.sival_int = 382;
	sigqueue(pid, SIGRTMIN, val);
	printf("SIGRTMIN :      %d\n", SIGRTMIN);

	val.sival_int = 3838;
	sigqueue(pid, SIGRTMAX - 5, val);
	printf("SIGRTMAX - 5 :  %d\n", SIGRTMAX - 5);

	val.sival_int = 383;
	sigqueue(pid, SIGINT, val);
	printf("SIGINT :        %d\n", SIGINT);
	
	val.sival_int = 38;
	sigqueue(pid, SIGRTMAX, val);
	printf("SIGRTMAX :      %d\n", SIGRTMAX);

	val.sival_int = 381;
	sigqueue(pid, SIGABRT, val);
	printf("SIGABRT :       %d\n", SIGABRT);

	printf("press any key to send SIGCONT to interrupt suspend.\n");
	getchar();
	sigqueue(pid, SIGCONT, val);
	printf("SIGCONT :       %d\n", SIGCONT);

	return 0;
}
