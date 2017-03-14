//conclude: file lock is per process, thread can overwrite file lock set by another thread in the same process. if close file descriptor from
//dup2 or from calling open, file lock gained through original open 
//will be lost cause file lock is per process attribute, and kernel
//tell owner of lock at granularity of process, not descriptor

#include "assist.h"

#define FILE_PATH_TO_LOCK "file_lock3.test"
#define LOCK_START 5
#define LOCK_END 10

int fd, new_fd;
int dup_fd = 38;

void *thread (void *arg)
{
	struct flock fl;

	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = LOCK_START;
	fl.l_len = LOCK_END - LOCK_START + 1;
	fl.l_type = F_WRLCK;
	if (fcntl(fd, F_SETLK, &fl) == -1){
		fprintf(stderr, "thread fcntl to set lock failed, errno:%d, error:%s", errno, strerror(errno));
	}
	else printf("thread set write lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);
}

void sig_usr1 (int signo)
{
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_usr1;
	if (sigaction(SIGUSR1, &act, NULL) == -1) p_err("sigaction failed.");

	if ((fd = open(FILE_PATH_TO_LOCK, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");
	if (dup2(fd, dup_fd) == -1) p_err("dup2 failed.");
	if ((new_fd = open(FILE_PATH_TO_LOCK, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");

	struct flock fl;
	fl.l_type = F_RDLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = LOCK_START;
	fl.l_len = LOCK_END - LOCK_START + 1;
	if (fcntl(fd, F_SETLK, &fl) == -1) p_err("parent main fcntl to set lock failed.");
	else printf("parent main set read lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);

	pthread_t tid;
	int ret;
	if (ret = pthread_create(&tid, NULL, thread, (void*)0)) t_err("pthread_create failed.", ret);
	if (ret = pthread_join(tid, NULL)) t_err("pthread_join failed.", ret);

	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork failed.");
	if (pid == 0){
		fl.l_type = F_WRLCK;
		if (fcntl(fd, F_SETLK, &fl) == -1) fprintf(stderr, "child fcntl to set lock failed, errno:%d, error:%s", errno, strerror(errno));
		else printf("child set write lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);
		pause();
		printf("child after pause.\n");
		if (fcntl(fd, F_SETLK, &fl) == -1) fprintf(stderr, "child fcntl2 to set lock failed, errno:%d, error:%s", errno, strerror(errno));
		else printf("child set write lock from pos %ld to %ld.\n", LOCK_START, LOCK_END);
		return 0;
	}else{
		sleep(3);
		//close(38);
		close(new_fd);
		printf("send child with SIGUSR1 to signify.\n");
		kill(pid, SIGUSR1);
		waitpid(pid, NULL, 0);
	}

	return 0;
}
