//1.if given host name or host addr,
//get and print host entry from file
// /etc/hosts, and then(or host name
// or addr is not given) print all 
// entries in that file
//2.usage: progname [host_name|host_port]

#include "assist.h"

int get_addr_type (char *addr_buf)
{
	int has_non_digit = 0;
	for (char *ch = addr_buf; *ch != '\0'; ch++){
		if (*ch == ':') return 2; //ipv6
		else if (*ch == '.') continue;
		else if (!isdigit(*ch)) has_non_digit = 1;
	}
	if (has_non_digit == 1) return 0; //not ip addr
	else return 1; //ipv4
}

void print_addrinfo (struct addrinfo *list)
{
	struct addrinfo *p;
	char addr_buf[INET6_ADDRSTRLEN];
	for (p = list; p != NULL; p = p->ai_next){
		printf("ai_flags:%d\n", p->ai_flags);
		printf("ai_family:%s\n", p->ai_family == AF_INET ? "AF_INET" : "AF_INET6");
		printf("ai_socktype:%s\n", p->ai_socktype == SOCK_DGRAM ? "SOCK_DGRAM" : "SOCK_STREAM");
		printf("ai_protocol:%d\n", p->ai_protocol);
		errno = 0;
		void *inaddr;
		if (p->ai_family == AF_INET){
			inaddr = &((struct sockaddr_in*)p->ai_addr)->sin_addr;
		}
		else if (p->ai_family == AF_INET6){
			inaddr = &((struct sockaddr_in6*)p->ai_addr)->sin6_addr;
		}
		if (inet_ntop(p->ai_family, inaddr, addr_buf, INET6_ADDRSTRLEN) == NULL) printf("ai_addr:failed.\n");
		if (errno == 0) printf("ai_addr:%s\n", addr_buf);
		printf("ai_canonname:%s\n", p->ai_canonname);
		printf("\n");
		if (p->ai_next == p) break;
	}
}

void print_nameinfo (char *host, char *serv)
{
	printf("host:%s\n", host);
	printf("serv:%s\n", serv);
	printf("\n");
}

int main (int argc, char *argv[])
{
	if (argc != 2){
		fprintf(stderr, "usage:usage: progname [host_name|host_port].\n");
		exit(1);
	}
	int addr_type;
	addr_type = get_addr_type (argv[1]);
	if (addr_type == 0){
		struct addrinfo host_item, *list;
		int ret;
		memset(&host_item, 0, sizeof(struct addrinfo));
		host_item.ai_family = AF_UNSPEC;
		if ((ret = getaddrinfo(argv[1], NULL, &host_item, &list))){
			fprintf(stderr, "getaddrinfo failed, errno:%d, error:%s\n", ret, gai_strerror(ret));
			exit(1);
		}
		print_addrinfo(list);
	}
	else{
		struct sockaddr sa;
		char addr_buf[INET6_ADDRSTRLEN];
		char host[NI_MAXHOST];
		char service[NI_MAXSERV];
		void *inaddr;
		memset(&sa, 0, sizeof(sa));
		if (addr_type == 1){
			sa.sa_family = AF_INET;
			inaddr = &(((struct sockaddr_in*)&sa)->sin_addr);
		}
		else if (addr_type == 2){
			sa.sa_family = AF_INET6;
			inaddr = &(((struct sockaddr_in6*)&sa)->sin6_addr);
		}
		if (inet_pton(sa.sa_family, argv[1], inaddr) != 1){
			fprintf(stderr, "inet_pton failed.");
			exit(2);
		}
		if (getnameinfo(&sa, sa.sa_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), host, sizeof(host), service, sizeof(service), NI_NOFQDN)){
			fprintf(stderr, "getnameinfo failed.\n");
			exit(3);
		}
		print_nameinfo(host, service);
	}

	return 0;
}
