/*
** client.c -- a stream socket client demo
*/
#include "assist.h"

#define PORT "3838" // the port client will be connecting to 
#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void *thread (void *arg)
{
	char buf[MAXDATASIZE];
	int sockfd = (int)(long)arg;
	int maxfd = sockfd;
	fd_set fds, write_fds;
	FD_ZERO(&fds);
	FD_SET(sockfd, &fds);
	ssize_t bytes;
	while (1){
		write_fds = fds;
		while (select(maxfd + 1, NULL, &write_fds, NULL, NULL) == -1){
			if (errno == EINTR) continue;
			else p_err("select failed.");
		}
		printf("type message to send to server.\n");
    		while ((bytes = read(STDIN_FILENO, buf, MAXDATASIZE)) == -1){
			if (errno == EINTR) continue;
			else p_err("read user input failed.");
		}
		while (send(sockfd, buf, bytes, 0) == -1){
			if (errno == EINTR) continue;
			else p_err("send failed.");
		}
	}
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo); // all done with this structure
    fd_set fds, read_fds;
    int fd_max;
    FD_ZERO(&fds);
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &fds);
    fd_max = sockfd;
    pthread_t tid;
    if (pthread_create(&tid, NULL, thread, (void*)(long)sockfd)) p_err("pthread_create failed.");
    while (1){
	    read_fds = fds;
	    ssize_t bytes;
	    while (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1){
		    if (errno == EINTR) continue;
		    else perror("select failed.");
	    }
	    if ((bytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) p_err("recv failed.");
	    else if (bytes == 0){
		    perror("server hung up.\n");
		    exit(0);
	    }else{
		    printf("msg from server: ");
		    for (ssize_t i = 0; i < bytes; i++) printf("%c", buf[i]);
		    printf("\n");
	    }
    }
    close(sockfd);
    return 0;
}
