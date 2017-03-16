#include "assist.h"

#define MAX_CONNECT_NUM 20
#define CLIENT_MESSAGE_MAX_LEN 512
#define DEFAULT_PORT "3838"
#define DEFAULT_ADDR NULL
#define INITIAL_FDS_NUM 256
#define IO_BUFFER_LEN 256

enum rwop {unused, read_pending, write_pending};
struct aiocb_t{
	struct aiocb aiocb;
	enum rwop op;
}
struct aiocbs{
	int fd;
	struct aiocb_t read_aio;
	struct aiocb_t write_aio;
}
static struct aiocb_t *aios;
static size_t aios_num;
static size_t aios_size;
static pthread_mutex_t mutex;

void init_addrinfo (struct addrinfo *addr);
void sig_aio_read (int signo, siginfo_t *info, void *arg);
void sig_aio_write (int signo, siginfo_t *info, void *arg);
void *aio_write_thread (void *arg);
void *aio_read_thread (void *arg)
void make_thread (pthread_t *tid, void *(*thread)(void *arg), void *arg);
void *get_in_addr (struct sockaddr *sa);
void init_mutex (void);

int main (int argc, char *argv[])
{
	struct addrinfo *servinfo, supply_servinfo;
	int ret;
	char lexical_addr[INET6_ADDRSTRLEN];
	char *addr, *port;

	if (argc > 3) err("usage: progname [bind_addr] bind_port_number.");
	init_addrinfo(&supply_servinfo);
	if (argc == 1){
		port = DEFAULT_PORT;
		addr = DEFAULT_ADDR;
	}else if (argc == 2){
		port = argv[1];
		addr = DEFAULT_ADDR;
	}else{
		addr = argv[1];
		port = argv[2];
	}
	if (ret = getaddrinfo(addr, port, &supply_servinfo, &servinfo)){
		fprintf(stderr, "getaddrinfo failed, network errno:%d, error:%s\n", ret, gai_strerror(ret));
		return 1;
	}

	struct addrinfo *p;
	for (p = servinfo; p != NULL; p = p->ai_next){
		if ((listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			fprintf(stderr, "socket failed.\n");
			continue;
		}
		int yes = 1;
		if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
			if (close(listen_fd) == -1) p_err("close socket failed to setsockopt failed.");
			fprintf(stderr, "setsockopt failed.\n");
			continue;
		}
		if (bind(listen_fd, p->ai_addr, p->ai_addrlen) == -1){
			int family;
			if (close(listen_fd) == -1) p_err("close socket failed to bind failed.");
			if (inet_ntop(p->ai_family, get_in_addr(p->ai_addr), lexical_addr, INET6_ADDRSTRLEN) == NULL){
				p_err("inet_ntop failed.");
			}
			fprintf(stderr, "bind to addr:%s failed.\n", lexical_addr);
			continue;
		}
		break;
	}
	if (p == NULL){
		fprintf(stderr, "server: failed to bind addr to %s:%s.", addr, port);
		exit(2);
	}
	freeaddrinfo(servinfo);
	if (listen(listen_fd, MAX_CONNECT_NUM) == -1) p_err("listen failed.");

	sigset_t mask;
	sigfillset(&mask);
	if (ret = pthread_sigmask(SIG_BLOCK, &mask, NULL)) p_err("pthread_sigmask failed.");
	init_mutex();
	make_thread(&tid, aio_read_thread, (void*)read_thread);
	make_thread(&tid, aio_write_thread, (void*)write_thread);

	printf("select server waiting for connection...\n");
	struct sockaddr_storage client_addr;
	char client_msg[CLIENT_MESSAGE_MAX_LEN];
	socklen_t client_addrlen;
	int new_fd;
	client_addrlen = sizeof(client_addr);
	while (1){
		if ((new_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addrlen)) == -1){
			fprintf(stderr, "accept failed, errno:%d, error:%s\n", errno, strerror(errno));
			continue;
		}
		////////////////
		printf("select server: new connection from %s on sockfd %d.\n", inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr), lexical_addr, INET6_ADDRSTRLEN), new_fd);
	}

	return 0;
}

void init_addrinfo (struct addrinfo *addr)
{
	addr->ai_flags = AI_PASSIVE;
	addr->ai_family = AF_UNSPEC;
	addr->ai_socktype = SOCK_STREAM;
	addr->ai_protocol = 0;
	addr->ai_addrlen = 0;
	addr->ai_addr= NULL;
	addr->ai_canonname= NULL;
	addr->ai_next= NULL;
}

void *aio_read_thread (void *arg)
{
	sigset_t mask;
	int ret;
	struct sigaction act;

	sigfillset(&mask);
	sigdelset(&mask, SIGRTMIN);
	if (ret = pthread_sigmask(SIG_SETMASK, &mask, NULL)) t_err("pthread_sigmask failed.", ret);
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = sig_aio_read;
	if (sigaction(SIGRTMIN, &act, NULL) == -1) p_err("sigaction failed.");

	while (1){
		pause();
	}
	pthread_exit(0);
}

void *aio_write_thread (void *arg)
{
	sigset_t mask;
	int ret;
	struct sigaction act;

	sigfillset(&mask);
	sigdelset(&mask, SIGRTMAX);
	if (ret = pthread_sigmask(SIG_SETMASK, &mask, NULL)) t_err("pthread_sigmask failed.", ret);
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = sig_aio_write;
	if (sigaction(SIGRTMAX, &act, NULL) == -1) p_err("sigaction failed.");

	while (1){
		pause();
	}
	pthread_exit(0);
}

void make_thread (pthread_t *tid, void *(*thread)(void *arg), void *arg)
{
	int ret;
	pthread_attr_t attr;
	if (ret = pthread_attr_init(&attr)) p_err("pthread_mutexattr_init failed.");
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (ret = pthread_create(tid, &attr, thread, arg)) p_err("pthread_create failed.");
	pthread_attr_destroy(&attr);
}

void *get_in_addr (struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) return &(((struct sockaddr_in*)sa)->sin_addr);
	else return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void init_mutex (void)
{
	int ret;
	pthread_mutexattr_t attr;
	if (ret = pthread_mutexattr_init(&attr)) t_err("pthread_mutexattr_init failed.", ret);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	if (ret = pthread_mutex_init(&mutex, &attr)) t_err("pthread_mutex_init failed.", ret);
	pthread_mutexattr_destroy(&attr);
}

void sig_aio_read (int signo, siginfo_t *info, void *arg)
{
	int ret;
	size_t pos = (size_t)arg;

	if ((ret = aio_error(&aiocb_t->aiocb)) == EINPROGRESS || ret == ECANCELED) return;
	else if (ret){
		if (ret == EINTR){
			if (signo == SIGRTMIN) aio_read(&aios[pos].read_aio);
			else aio_write(&aio[pos].write_aio);
			return 0;
		}
		else{
			fprintf("aio_error indicate %s failed on fd %d, errno:%d, error:%s, close it\n", signo == SIGRTMIN ? "read" : "write", aiocb_t->aiocb.aio_fildes, ret, strerror(ret));
		}
	}
	else if (aiocb_t->type == aio_type_write) return;
	ssize_t bytes;
	if ((bytes = aio_return(aiocb_t->read_aiocb)) == -1) p_err("aio_return failed on reading sock %d, errno:%d, error:%s.\n", aiocb_t->aiocb.aio_fildes, errno, error);
	else if (bytes == 0){
		printf("aio server: socket %d hung up.\n", aiocb_t->aiocb.aio_fildes);
		del_from_fds(aiocb_t->aiocb.aio_fildes);
	}else{
		send_to_fds (, );
	}
}

void del_from_fds (int fd)
{
	int ret;
	if (ret = pthread_mutex_lock(&mutex)) t_err("pthread_mutex_lock failed.", ret);
	int num = fds_num;
	for (size_t i = 0; i < fds_size; i++){
		if (fds[i].fd == fd){
		}
	}
	pthread_mutex_unlock(&mutex);
}

void send_to_fds (int fd, const char *s)
{
	int ret;

	if (ret = pthread_mutex_lock(&mutex)) t_err("pthread_mutex_lock failed.", ret);
	int num = fds_num;
	for (size_t i = 0; i < fds_size && num > 0; i++){
		if (fds[i].fd != -1){
			num--;
			if (fds[i].fd != fd && fds[i].fd != listen_fd){
				fds[i].write_aiocb = s;
				if (aio_write(fds[i].write_aiocb) == -1) fprintf(stderr, "aio_write queued failed, errno:%d, error:%s", errno, strerror(errno));
			}
		}
	}
	pthread_mutex_unlock(&mutex);
}

int allocate_new_fd (struct fds_t *fds_t, int fd)
{
	fds_t->fd = fd;IO_BUFFER_LEN
	if ((fds_t->read_aiocb = malloc(sizeof(struct aiocb))) == NULL) return 1;
	if ((fds_t->write_aiocb = malloc(sizeof(struct aiocb))) == NULL) return 1;
	if ((fds_t->read_aiocb.aio_buf = malloc(IO_BUFFER_LEN)) == NULL) return 2;
	if ((fds_t->write_aiocb.aio_buf = malloc(IO_BUFFER_LEN)) == NULL) return 2;
	fds_t->read_aiocb.aio_fildes = fd;
	fds_t->write_aiocb.aio_fildes = fd;
	fds_t->read_aiocb.aio_offset = fd;
	fds_t->write_aiocb.aio_offset = fd;
	fds_t->read_aiocb.aio_nbytes = IO_BUFFER_LEN;
	fds_t->write_aiocb.aio_nbytes = IO_BUFFER_LEN;
	fds_t->read_aiocb.aio_reqprio = 0;
	fds_t->write_aiocb.aio_reqprio = 0;
	fds_t->read_aiocb.aio_lio_opcode = 0;
	fds_t->write_aiocb.aio_lio_opcode = 0;
	fds_t->read_aiocb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	fds_t->write_aiocb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	fds_t->read_aiocb.aio_sigevent.sigev_signo = SIGRTMIN;
	fds_t->write_aiocb.aio_sigevent.sigev_signo = SIGRTMAX;
	fds_t->read_aiocb.aio_sigevent.sigev_value.sival_ptr = fds_t;
	fds_t->write_aiocb.aio_sigevent.sigev_value.sival_ptr = fds_t;
	fds_t->read_aiocb.aio_sigevent.sigev_notify_function = NULL;
	fds_t->write_aiocb.aio_sigevent.sigev_notify_function = NULL;
	fds_t->read_aiocb.aio_sigevent.sigev_notify_attributes = NULL;
	fds_t->write_aiocb.aio_sigevent.sigev_notify_attributes = NULL;
	return 0;
}
