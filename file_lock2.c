//from apue: Note that POSIX.1 doesn’t specify what 
//happens when one process read locks a range of a file, 
//a second process blocks while trying to get a write 
//lock on the same range, and a third processes then 
//attempts to get another read lock on the range. If the 
//third process is allowed to place a read lock on the 
//range just because the range is already read locked, then 
//the implementation might starve processes with pending write locks.
//just test it.

//conclude:on linux, as apue said!

#include "assist.h"

#define FILE_PATH_TO_LOCK "file_lock2.test"
#define LOCK_START 5
#define LOCK_END 10

jmp_buf jmp;

void sig_usr1 (int signo)
{
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_usr1;
	if (sigaction(SIGUSR1, &act, NULL) == -1) p_err("sigaction failed.");

	int fd;
	if ((fd = open(FILE_PATH_TO_LOCK, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");

	struct flock fl;
	fl.l_type = F_RDLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = LOCK_START;
	fl.l_len = LOCK_END - LOCK_START + 1;
	if (fcntl(fd, F_SETLK, &fl) == -1) p_err("fcntl to set lock failed.");
	else printf("parent set read lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);

	pid_t pid1, pid2;
	if ((pid1 = fork()) == -1) p_err("fork 1 failed.");
	if (pid1 == 0){
		fl.l_type = F_WRLCK;
		if (fcntl(fd, F_SETLKW, &fl) == -1){
			fprintf(stderr, "child1 fcntl to set lock failed, errno:%d, error:%s", errno, strerror(errno));
			pause();
		}
		else printf("child1 write lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);
		printf("child1 get lock.\n");
		kill(getppid(), SIGUSR2);
		exit(0);
	}

	if ((pid2 = fork()) == -1) p_err("fork 2 failed.");
	if (pid2){
		int i = 0;
		while (i < 3){
			i++;
			pause();
			sleep(2);
			fl.l_type = F_UNLCK;
			if (fcntl(fd, F_SETLK, &fl) == -1) p_err("parent fcntl to unlock failed.");
			else printf("parent unlock pos %ld to %ld successful.\n", LOCK_START, LOCK_END);
			kill(pid2, SIGUSR1);
			fl.l_type = F_RDLCK;
			if (fcntl(fd, F_SETLKW, &fl) == -1) p_err("parent fcntl to set read lock failed.");
			else printf("parent set read lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);
			sleep(2);
		}
		printf("now kill child2 with SIGTERM.\n");
		kill(pid2, SIGTERM);
		fl.l_type = F_UNLCK;
		if (fcntl(fd, F_SETLK, &fl) == -1) p_err("parent fcntl to unlock failed.");
		else printf("parent unlock pos %ld to %ld successful.\n", LOCK_START, LOCK_END);
	}else{
		while (1){
			kill(getppid(), SIGUSR1);
			fl.l_type = F_RDLCK;
			if (fcntl(fd, F_SETLKW, &fl) == -1) p_err("child2 fcntl to set read lock failed.");
			else printf("child2 set read lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);
			pause();
			sleep(2);
			fl.l_type = F_UNLCK;
			if (fcntl(fd, F_SETLKW, &fl) == -1) p_err("child2 fcntl to unlock failed.");
			else printf("child2 unlock pos %ld to %ld successful.\n", LOCK_START, LOCK_END);
		}
	}

	return 0;
}
