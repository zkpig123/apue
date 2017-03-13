#include "assist.h"

#define FILE_PATH_TO_LOCK "file_lock1.test"

int main (void)
{
	int fd;
	if ((fd = open(FILE_PATH_TO_LOCK, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");
	struct flock fl;
	fl.l_type = F_RDLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 5;
	fl.l_len = 3;
	if (fcntl(fd, F_SETLKW, &fl) == -1) p_err("fcntl to set lock failed.");

	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork failed.");
	if (pid == 0){
		fl.l_type = F_RDLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = 3;
		fl.l_len = 7;
		if (fcntl(fd, F_GETLK, &fl) == -1) p_err("fcntl throught F_GETLK failed.");
		if (fl.l_type == F_UNLCK) printf("can get read lock on 3 to 7.\n");
		else{
			printf("can't get read from 3 to 7, because %d to %d is %s locked.\n", fl.start, fl.l_start + fl.l_len - 1, 
		}
	}else{
		pause(); //wait for child
	}

	return 0;
}
