//1. test whether mandatory lock is enabled if file has setgid enabled, 
//and gid run permission disabled, and file system support mandatory 
//lock and is enabled
//2. whether unlink could ignore mandatory lock
//3. test should under file system with mandatory lock enabled, on linux,
//enable method: mount -o remount,mand. testing file should has setgid
//enabled and group run permission disabled

//conclude: is enabled; unlink could circumvent mandatory lock

#include "assist.h"

#define FILE_PATH_TO_LOCK "file_lock_mandatory1.test"
#define LOCK_START 0
#define LOCK_END 1000
#define CONTENT_TO_WRITE "hello world.!"
#define CONTENT_LEN sizeof(CONTENT_TO_WRITE) - 1
#define CONTENT_TO_WRITE_FROM_GRANDCHILD "TEST FROM GRANDCHILD."
#define CONTENT_LEN_FROM_GRANDCHILD sizeof(CONTENT_TO_WRITE_FROM_GRANDCHILD) - 1

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
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = LOCK_START;
	fl.l_len = LOCK_END - LOCK_START + 1;
	if (fcntl(fd, F_SETLK, &fl) == -1) p_err("parent main fcntl to set lock failed.");
	else printf("parent main set read lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);

	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork failed.");
	if (pid){ //parent
		sleep(1);
		int ch = 0;
		do {
			printf("type K to release lock on file.\n");
		}while (ch != EOF && (ch = getchar()) != 'K');
		fl.l_type = F_UNLCK;
		if (fcntl(fd, F_SETLK, &fl) == -1) p_err("parent main fcntl to set lock failed.");
		else printf("parent main release read lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);
		if (close(fd) == -1) p_err("parent close fd failed.");
		waitpid(pid, NULL, 0);
		return 0;
	}

	//child
	ssize_t written;
	printf("child now write %s to file %s.\n", CONTENT_TO_WRITE, FILE_PATH_TO_LOCK);
	if ((written = write(fd, CONTENT_TO_WRITE, CONTENT_LEN)) == -1){
		fprintf(stderr, "child write failed, errno:%d, error:%s.\n", errno, strerror(errno));
		return 0;
	}else{
		if (written != CONTENT_LEN) printf("child only write part of content, written %ld bytes, mandatory lock is not held by parent\n", (long)written);
		else printf("child write file successful.\n"); 
	}

	fl.l_type = F_WRLCK;
	if (fcntl(fd, F_SETLKW, &fl) == -1) p_err("child fcntl to set write lock failed.");
	else{
		if (LOCK_END == 0) printf("child set write lock from pos %ld to %s.\n", LOCK_START, "INFINITY");
		else printf("child set write lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);
	}

	if ((pid = fork()) == -1) p_err("child fork failed.");
	if (pid == 0){
		if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) p_err("grandchild set file to non block failed.");
		printf("grandchild now write to file.\n");
		ssize_t written;
		written = write(fd, CONTENT_TO_WRITE_FROM_GRANDCHILD, CONTENT_LEN_FROM_GRANDCHILD);
		printf("after issuing nonblock write.\n");
		printf("check whether content is written to file, if content has written into the file, and then press any key to continue.\n");
		getchar();
		if (close(fd) == -1) p_err("grandchild close file failed.");
		printf("\npress any key to test mandatory on unlink func: grandchild will now unlink file which is mandatorily locked by child.\n");
		getchar();
		if (unlink(FILE_PATH_TO_LOCK) == -1){
			fprintf(stderr, "grandchild unlink file failed, errno:%d, error:%s.\n", errno, strerror(errno));
		}else{
			printf("grandchild unlink file successful, meaning unlink could ignore mandatory lock!\n");
			printf("see whether file has been unlinked, and then press any key to continue.\n");
			getchar();
		}
		return 0;
	}else{
		waitpid(pid, NULL, 0);
	}
	return 0;
}
