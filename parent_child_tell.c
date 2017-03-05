//code from apue Figure 10.24 and Section 8.9

#include "assist.h"

static volatile sig_atomic_t sigflag; /* set nonzero by sig handler */
static sigset_t newmask, oldmask, zeromask;
static void sig_usr(int signo)
{
	sigflag = 1;
}
/* one signal handler for SIGUSR1 and SIGUSR2 */

void TELL_WAIT(void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_usr;

	if(sigaction(SIGUSR1, &act, NULL) == -1) p_err("sigaction 4 SIGUSR1 failed.");
	if(sigaction(SIGUSR2, &act, NULL) == -1) p_err("sigaction 4 SIGUSR2 failed.");
	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	sigaddset(&newmask, SIGUSR2);
	/* Block SIGUSR1 and SIGUSR2, and save current signal mask */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) p_err("sigprocmask block failed");
}

void TELL_PARENT(pid_t pid)
{
	kill(pid, SIGUSR2);
}
/* tell parent we’re done */

void WAIT_PARENT(void)
{
	while (sigflag == 0)
	sigsuspend(&zeromask);
	sigflag = 0;
	/* and wait for parent */
	/* Reset signal mask to original value */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) p_err("sigprocmask SIG_SETMASK error");
}
void TELL_CHILD(pid_t pid)
{
	kill(pid, SIGUSR1);
}
void WAIT_CHILD(void)
{
	while (sigflag == 0)
	sigsuspend(&zeromask);
	sigflag = 0;
	/* and wait for child */
	/* Reset signal mask to original value */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) p_err("sigprocmask SIG_SETMASK error");
}

int main (void)
{
	pid_t pid;
	TELL_WAIT();
	int fd;
	if ((fd = open("parent_child_tell.test", O_RDWR | O_CREAT | O_SYNC, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");
	char buf[1];
	buf[0] = 1;
	if (write(fd, buf, 1) != 1) p_err("write failed.");
	/* set things up for TELL_xxx & WAIT_xxx */
	if ((pid = fork()) < 0) {
		err("fork error");
	}else if (pid == 0) {
		/* child */
		/* child does whatever is necessary ... */
		while (1){
			ssize_t bytes;
			WAIT_PARENT();
			sleep(1);
			if (lseek(fd, 0, SEEK_SET) == (off_t)-1) p_err("lseek failed.");
			if ((bytes = read(fd, buf, 1)) != 1){
				if (bytes == 0) err("read 0 bytes.");
				else p_err("read failed.");
			}
			printf("child read: %d and add 1 to it\n", buf[0]);
			buf[0]++;
			if (lseek(fd, 0, SEEK_SET) == (off_t)-1) p_err("lseek failed.");
			if ((bytes = write(fd, buf, 1)) != 1) p_err("child write failed.");
			TELL_PARENT(getppid());
		}
		/* tell parent we’re done */
		/* and wait for parent */
		/* and the child continues on its way ... */
		exit(0);
	}
	/* parent does whatever is necessary ... */
	while (1){
		sleep(1);
		ssize_t bytes;
		if (lseek(fd, 0, SEEK_SET) == (off_t)-1) p_err("lseek failed.");
		if ((bytes = read(fd, buf, 1)) != 1){
			if (bytes == 0) err("read 0 bytes.");
			else p_err("read failed.");
		}
		printf("parent read: %d and add 1 to it\n", buf[0]);
		buf[0]++;
		if (lseek(fd, 0, SEEK_SET) == (off_t)-1) p_err("lseek failed.");
		if ((bytes = write(fd, buf, 1)) != 1) p_err("child write failed.");
		TELL_CHILD(pid);
		WAIT_CHILD();
	}
	/* tell child we’re done */
	/* and wait for child */
	/* and the parent continues on its way ... */
	exit(0);
}
