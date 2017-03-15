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
    fd_set fds, read_fds, write_fds;
    int fd_max, read_ready_num, write_ready_num;
    FD_ZERO(&fds);
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_SET(listen_fd, &fds);
    fd_max = sockfd;
    while (1){
	    read_fds = fds;
	    write_fds = fds;
	    ssize_t bytes;
	    while ((read_ready_num = select(fd_max + 1, &read_fds, &write_fds, NULL, NULL)) == -1){
		    if (errno == EINTR) continue;
		    else perror("select failed.");
	    }
	    write_ready_num = read_ready_num;
	    for (int i = 0; i <= fd_max && ready_num > 0; i++){
		    if (FD_ISSET(i, &read_fds)){
			    read_ready_num--;
			    if ((bytes = recv(i, buf, MAXDATASIZE, 0)) == -1) p_err("recv failed.");
			    else if (bytes == 0){
				    perror("server hung up.\n", i);
			    }else{
				    printf("msg from server: ");
				    for (ssize_t i = 0; i < bytes; i++) printf("%c", buf[i]);
				    printf("\n");
			    }
		    }
	    }
	    for (int i = 0; i <= fd_max && write_ready_num > 0; i++){
		    if (FD_ISSET(i, &write_fds)){
			    write_ready_num--;
			    printf("type message to send to server in 3 seconds.");
		    }
	    }
    }
    /*if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
       exit(1);
    }
    buf[numbytes] = '\0';
    printf("client: received '%s'\n",buf);*/
    pthread_t tid;
    if (pthread_create(&tid, NULL, thread, (void*)sockfd)) p_err("pthread_create failed.");
    while (1){
	    ssize_t bytes;
	    printf("type msg to send to server.\n");
	    while ((bytes = read(STDIN_FILENO, buf, MAXDATASIZE)) == -1 && errno == EINTR) ;
	    if (bytes == 0) continue;
	    while (send(sockfd, buf, bytes, 0) == -1){
		    if (errno == EINTR) continue;
		    p_err("send failed.");
	    }
    }
    close(sockfd);
    return 0;
}
