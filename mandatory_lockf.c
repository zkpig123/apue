//test: whether lockf could mandatorily lock a file, need
//file with setgid, and group run permission disabled, and
//mandatory enabled on file system, how to enable: mount with
//option -o mand, or command 'mount -o remount,mand'

//conclude: lockf could supply both mandatory and advisory locks

#include "assist.h"

#define FILE_PATH_TO_LOCK "mandatory_lockf.test"
#define CONTENT_TO_WRITE "hello world."
#define CONTENT_LEN sizeof(CONTENT_TO_WRITE) - 1

int main (void)
{
	int fd;
	if ((fd = open(FILE_PATH_TO_LOCK, O_CREAT | O_RDWR | O_SYNC, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");

	struct stat fattr;
	if (fstat(fd, &fattr) == -1) p_err("fstat failed.");
	fattr.st_mode |= fattr.st_mode & ~S_IXGRP | S_ISGID;
	if (fchmod(fd, fattr.st_mode) == -1) p_err("fchmod failed.");

	if (lockf(fd, F_LOCK, 0) == -1) p_err("lockf failed.");

	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork failed.");
	if (pid == 0){
		ssize_t written;
		if ((written = write(fd, CONTENT_TO_WRITE, CONTENT_LEN)) == -1){
			fprintf(stderr, "child write failed, errno:%d, error:%s.\n", errno, strerror(errno));
			return 0;
		}else{
			printf("child write file successful, written %ld bytes, mandatory lock is not held by parent\n", (long)written);
		}
		return 0;
	}
	printf("press any key to let parent to release lock, and see whether child could write.\n");
	getchar();
	if (lockf(fd, F_ULOCK, 0) == -1) p_err("lockf release failed.");
	if (waitpid(pid, NULL, 0) == -1) p_err("wait failed.");

	return 0;
}
