//conclude: file lock is per process, thread can overwrite file lock set by another thread in the same process.

#include "assist.h"

#define FILE_PATH_TO_LOCK "file_lock_mandatory.test"
#define LOCK_START 5
#define LOCK_END 10

int fd, new_fd;
int dup_fd = 38;

void sig_usr1 (int signo)
{
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_usr1;
	if (sigaction(SIGUSR1, &act, NULL) == -1) p_err("sigaction failed.");

	if ((fd = open(FILE_PATH_TO_LOCK, O_CREAT | O_RDWR | O_SYNC, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");
	struct stat fattr;
	if (fstat(fd, &fattr) == -1) p_err("fstat failed.");
	fattr.st_mode |= fattr.st_mode & ~S_IXGRP | S_ISGID;
	if (fchmod(fd, fattr.st_mode) == -1) p_err("fchmod failed.");

	struct flock fl;
	fl.l_type = F_RDLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = LOCK_START;
	fl.l_len = LOCK_END - LOCK_START + 1;
	if (fcntl(fd, F_SETLK, &fl) == -1) p_err("parent main fcntl to set lock failed.");
	else printf("parent main set read lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);

	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork failed.");
	if (pid == 0){
		ssize_t written;
		if ((written = write(fd, "hello world!.", sizeof("hello world!." - 1))) == -1){
			fprintf(stderr, "child write failed, errno:%d, error:%s.\n", errno, strerror(errno));
			return 0;
		}else{
			printf("child write file successful, written %ld bytes, mandatory lock is not held by parent\n", (long)written);
		}
		return 0;
	}else{
		waitpid(pid, NULL, 0);
	}

	return 0;
}
