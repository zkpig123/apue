//conclude: though daemon has no login, on linux we got login name
//through getlogin...(test from some guys indicated that on 
//freebsd and Mac OS also could get login name cause login name
//is maintained in the process table and copied across a fork

#include "assist.h"

#define OPEN_MAX_GUESS 1024
#define UNIQUE_LOCK_FILE_PATH "/var/run/first_daemon.pid"
#define UNIQUE_LOCK_FILE_MODE S_IRUSR | S_IWUSR
#define LOG_FILE_PATH "first_daemon.log"

int lockfile (int fd);
int unlockfile (int fd);
void sig_hup (int signo);

int main (void)
{
	//change umask
	if (umask(0) == -1){
		syslog(LOG_ERR, "umask failed.");
		exit(1);
	}
	
	pid_t pid;
	//fork one time, then setsid, then fork second time
	if ((pid = fork()) == -1){
		syslog(LOG_ERR, "fork1 failed.");
		exit(2);
	}
	if (pid) return 0;
	if (setsid() == -1){
		syslog(LOG_ERR, "setsid failed.");
		exit(3);
	}
	if ((pid = fork()) == -1){
		syslog(LOG_ERR, "fork2 failed.");
		exit(4);
	}
	if (pid) return 0;
	
	//change current working directory
	if (chdir("/") == -1){
		syslog(LOG_ERR, "chdir to / failed.");
		exit(5);
	}
	
	long open_max;
	//close descriptor
#ifdef OPEN_MAX
	open_max = OPEN_MAX;
#elif defined(_SC_OPEN_MAX)
	open_max = sysconf(_SC_OPEN_MAX);
	if (open_max == -1){
		syslog(LOG_ERR, "sysconf failed.");
		exit(6);
	}
#else
	open_max = OPEN_MAX_GUESS;
#endif
	for (long i = 0; i < open_max; i++){
		if (close(i) == -1){
			if (errno == EBADF) continue;
			else if (errno == EINTR){
				i--;
				continue;
			}
			else{
				syslog(LOG_ERR, "close failed.");
				exit(7);
			}
		}
	}
	
	//redirect descriptor 0,1,2 to /dev/null
	if (open("/dev/null", O_RDWR) == -1){
		syslog(LOG_ERR, "open /dev/null failed.");
		exit(8);
	}
	while (dup2(0, 1) == -1 && errno == EINTR) ;
	while (dup2(0, 2) == -1 && errno == EINTR) ;

	//lock unique file
	int unique_fd;
	if ((unique_fd = open(UNIQUE_LOCK_FILE_PATH, O_CREAT | O_RDWR, UNIQUE_LOCK_FILE_MODE)) == -1){
		syslog(LOG_ERR, "open unique flock file failed.");
		exit(8);
	}
	if (lockfile(unique_fd)){
		if (errno == EACCES || errno == EAGAIN){
			syslog(LOG_ERR, "other lock on unique lock file exists. another instance of our daemon exist?");
		}
		close(unique_fd);
		syslog(LOG_ERR, "lock unique file failed.");
		exit(9);
	}
	if (ftruncate(unique_fd, 0) == -1){
		syslog(LOG_ERR, "lock unique file failed.");
		exit(10);
	}
#define PID_T_BITS_MAX 20
	char pids[PID_T_BITS_MAX];
	int ret;
	if ((ret = snprintf(pids, PID_T_BITS_MAX, "%ld", getpid())) == -1 || ret + 1 > PID_T_BITS_MAX){
		syslog(LOG_ERR, "buffer for pid not enough.");
		exit(11);
	}
	if (write(unique_fd, pids, ret) != ret){
		syslog(LOG_ERR, "write fd to unique lock file failed.");
		exit(12);
	}
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_hup;
	if (sigaction(SIGHUP, &act, NULL) == -1){
		syslog(LOG_ERR, "sigaction 4 sighup failed.");
		exit(13);
	}

	openlog("first_daemon", LOG_PERROR | LOG_PID, LOG_USER);
	syslog(LOG_NOTICE, "our first daemon is running now.");
	syslog(LOG_LOCAL0 || LOG_INFO, "second message from our first daemon is running now.");
	int fd;
	if ((fd = open(LOG_FILE_PATH, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR)) == -1){
		syslog(LOG_ERR, "open log file failed.");
		exit(16);
	}
	char *login;
	if ((login = getlogin()) == NULL){
		syslog(LOG_ERR, "getlogin failed.");
		exit(17);
	}
	if (dprintf(fd, "login user is %s\n", login) < 0){
		syslog(LOG_ERR, "dprintf failed.");
		exit(18);
	}
	while (1);

	//exit cleanup
	if (unlockfile(unique_fd)){
		syslog(LOG_ERR, "unlock lock on unique lock file failed.");
		exit(14);
	}
	if (unlink(UNIQUE_LOCK_FILE_PATH) == -1){
		syslog(LOG_ERR, "unlink unique lock file failed.");
		exit(15);
	}

	return 0;
}

int lockfile (int fd)
{
	struct flock fl;
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	if (fcntl(fd, F_SETLK, &fl) == -1) return 1;
	else return 0;
}

int unlockfile (int fd)
{
	struct flock fl;
	fl.l_type = F_UNLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	if (fcntl(fd, F_SETLK, &fl) == -1) return 1;
	else return 0;
}

void sig_hup (int signo)
{
	syslog(LOG_NOTICE, "in signal handler for sighup...do reread operation.");
}
