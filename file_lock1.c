#include "assist.h"

#define FILE_PATH_TO_LOCK "file_lock1.test"
#define PARENT_LOCK_START1 4
#define PARENT_LOCK_END1 8
#define PARENT_LOCK_START2 7
#define PARENT_LOCK_END2 12
#define CHILD_LOCK_START 7
#define CHILD_LOCK_END 8

void sig_handler (int signo)
{
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if (sigaction(SIGUSR1, &act, NULL) == -1) p_err("sigaction failed.");
	if (sigaction(SIGUSR2, &act, NULL) == -1) p_err("sigaction failed.");

	int fd;
	if ((fd = open(FILE_PATH_TO_LOCK, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");
	struct flock fl;
	fl.l_type = F_RDLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = PARENT_LOCK_START1;
	fl.l_len = PARENT_LOCK_END1 - PARENT_LOCK_START1 + 1;
	if (fcntl(fd, F_SETLKW, &fl) == -1) p_err("fcntl to set lock failed.");
	else printf("parent read lock from pos %ld to %ld.\n", PARENT_LOCK_START1, PARENT_LOCK_END1);
	fl.l_type = F_WRLCK;
	fl.l_start = PARENT_LOCK_START2;
	fl.l_len = PARENT_LOCK_END2 - PARENT_LOCK_START2 + 1;
	if (fcntl(fd, F_SETLKW, &fl) == -1) p_err("fcntl to set lock failed.");
	else printf("parent write lock from pos %ld to %ld.\n", PARENT_LOCK_START2, PARENT_LOCK_END2);

	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork failed.");
	if (pid == 0){
		char *locktype = NULL;
		pid_t ppid = getppid();
		fl.l_type = F_RDLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = CHILD_LOCK_START;
		fl.l_len = CHILD_LOCK_END - CHILD_LOCK_START + 1;
		if (fcntl(fd, F_GETLK, &fl) == -1) p_err("fcntl throught F_GETLK failed.");
		if (fl.l_type == F_UNLCK){
			printf("child can get read lock on %ld to %ld.\n", CHILD_LOCK_START, CHILD_LOCK_END);
			fl.l_type = F_WRLCK;
			if (fcntl(fd, F_GETLK, &fl) == -1) p_err("child fcntl throught F_SETLK failed.");
			if (fl.l_type == F_UNLCK) printf("child can get write lock on %ld to %ld.\n", CHILD_LOCK_START, CHILD_LOCK_END);
			else {
				if (fl.l_type == F_RDLCK) locktype = "read lock";
				else locktype = "write lock";
				printf("child can't get write lock from %ld to %ld, because %d to %d is %s locked.\n", CHILD_LOCK_START, CHILD_LOCK_END, fl.l_start, fl.l_start + fl.l_len - 1, locktype);
			}
		}
		else{
			if (fl.l_type == F_RDLCK) locktype = "read lock";
			else locktype = "write lock";
			printf("child can't get read from %ld to %ld, because %d to %d is %s locked.\n", CHILD_LOCK_START, CHILD_LOCK_END, fl.l_start, fl.l_start + fl.l_len - 1, locktype);
		}
		if (kill(ppid, SIGUSR1) == -1) p_err("child send sigusr1 to parent failed.");
	}else{
		waitpid(pid, NULL, 0);
	}

	return 0;
}
