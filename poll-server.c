#include "assist.h"

#define MAX_CONNECT_NUM 20
#define CLIENT_MESSAGE_MAX_LEN 512
#define DEFAULT_PORT "3838"
#define DEFAULT_ADDR NULL
#define INITIAL_POLL_FD_NUM 256

void init_addrinfo (struct addrinfo *addr);
void sig_chld (int signo);
void *thread (void *arg);
void make_thread (pthread_t *tid, void *(*thread)(void *arg), void *arg);
void *get_in_addr (struct sockaddr *sa);

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
	int listen_fd;
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

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_chld;
	if (sigaction(SIGCHLD, &act, NULL) == -1) p_err("sigaction failed.");
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	if (ret = pthread_sigmask(SIG_BLOCK, &mask, NULL)) p_err("pthread_sigmask failed.");
	pthread_t tid;
	make_thread(&tid, thread, (void*)0); //thread to process sigchld

	printf("select server waiting for connection...\n");
	struct sockaddr_storage client_addr;
	char client_msg[CLIENT_MESSAGE_MAX_LEN];
	socklen_t client_addrlen;
	int new_fd;
	client_addrlen = sizeof(client_addr);
	int ready_num;
	struct pollfd *poll_fds = NULL;
	size_t poll_fds_num;
	nfds_t nfds; //num of threads in struct pollfd list poll_fds
	if ((poll_fds = calloc(INITIAL_POLL_FD_NUM, sizeof(struct pollfd))) == NULL) err("calloc failed.");
	poll_fds_num = INITIAL_POLL_FD_NUM;
	poll_fds[0].fd = listen_fd;
	poll_fds[0].events = POLLIN;
	nfds = 1;
	for (size_t i = 1; i < poll_fds_num; i++) poll_fds[i].fd = -1;
	while (1){
		while ((ready_num = poll(poll_fds, poll_fds_num, -1)) == -1){
			if (errno == EINTR) continue;
			else perror("poll failed.");
		}
		for (struct pollfd *ready_fd = poll_fds; ready_fd < poll_fds + poll_fds_num && ready_num > 0; ready_fd++){
			if (ready_fd->fd >= 0 && ready_fd->revents){
				ready_num--;
				if (ready_fd->fd == listen_fd){
					if ((new_fd = accept(ready_fd->fd, (struct sockaddr*)&client_addr, &client_addrlen)) == -1){
						fprintf(stderr, "accept failed, errno:%d, error:%s\n", errno, strerror(errno));
						continue;
					}else{
						if (nfds >= poll_fds_num){
							poll_fds_num <<= 2;
							if (realloc(poll_fds, poll_fds_num * sizeof(struct pollfd)) == NULL) err("realloc failed.");
							struct pollfd *f;
							for (f = &poll_fds[nfds + 1]; f < &poll_fds[poll_fds_num - 1]; f++){
								f->fd = -1;
							}
							poll_fds[nfds].fd = new_fd;
							poll_fds[nfds++].events = POLLIN;
						}else{
							struct pollfd *t;
							for (t = poll_fds; t < poll_fds + poll_fds_num && t->fd != -1; t++) ;
							t->fd = new_fd;
							t->events = POLLIN;
						}
						printf("select server: new connection from %s on sockfd %d.\n", inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr), lexical_addr, INET6_ADDRSTRLEN), new_fd);
					}
				}else{
					ssize_t bytes;
					if ((bytes = recv(ready_fd->fd, client_msg, sizeof(client_msg), 0)) == -1){
						fprintf(stderr, "recv failed, errno:%d, error:%s.\n", errno, strerror(errno));
					}else if (bytes){
						printf("msg from client on sock %d: ", ready_fd->fd);
						fflush(stdout);
						while (write(STDOUT_FILENO, client_msg, bytes) == -1){
							if (errno == EINTR) continue;
							else p_err("write client msg to stdout failed.\n");
						}
						printf("\n");
						printf("send to other clients.\n");
						for (struct pollfd *q = poll_fds; q < poll_fds + poll_fds_num; q++){
							if (q->fd >= 0 && q->fd != listen_fd && q->fd != ready_fd->fd){
									if (send(q->fd, client_msg, bytes, 0) == -1) fprintf(stderr, "send msg to sock %d failed.\n", q->fd);
							}
						}
					}else{
						printf("select server: socket %d hung up.\n", ready_fd->fd);
						if (close(ready_fd->fd) == -1) p_err("close socket failed.");
						ready_fd->fd = -1;
						nfds--;
					}
				}
			}
		}

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

void sig_chld (int signo)
{
	while (waitpid(-1, NULL, WNOHANG) > 0) ;
}

void *thread (void *arg)
{
	sigset_t mask;
	int ret;

	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	if (ret = pthread_sigmask(SIG_UNBLOCK, &mask, NULL)) p_err("pthread_sigmask failed.");
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
