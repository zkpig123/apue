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

void print_host (struct hostent *host_item)
{
	printf("host name:%s\n", host_item->h_name);
	printf("alias list:\n");
	for (char **list = host_item->h_aliases; *list != NULL; list++) printf("           %s\n", *list);
	if (host_item->h_addrtype == AF_INET) printf("host addr type:%s\n", "AF_INET");
	else if (host_item->h_addrtype == AF_INET6) printf("host addr type:%s\n", "AF_INET6");
	else printf("host addr type:unknown.\n");
	printf("addr list:\n");
	for (char **list = host_item->h_addr_list; *list != NULL; list++){
		char ip_buf[INET6_ADDRSTRLEN];
		if (inet_ntop(host_item->h_addrtype, *list, ip_buf, sizeof(ip_buf)) == NULL){
			printf("          failed.\n");
			continue;
		}
		printf("%s\n", ip_buf);
	}
}

int main (int argc, char *argv[])
{
	struct hostent *host_item;
	if (argc == 2){
		int addr_type = get_addr_type(argv[1]);
		if (addr_type == 0){
			if ((host_item = gethostbyname(argv[1])) == NULL){
				fprintf(stderr, "gethostbyname failed. errno:%d, error:%s\n", h_errno, hstrerror(h_errno));
				exit(2);
			}
		}
		else{
			struct in_addr v4addr;
			struct in6_addr v6addr;
			void *inaddr;
			int family;
			socklen_t addr_len;
			if (addr_type == 1){ //ipv4
				family = AF_INET;
				inaddr = &v4addr;
				addr_len = sizeof(struct in_addr);
			}
			else{
				family = AF_INET6;
				inaddr = &v6addr;
				addr_len = sizeof(struct in6_addr);
			}
			if (inet_pton(family, argv[1], inaddr) != 1){
				fprintf(stderr, "inet_pton failed.\n");
				exit(3);
			}
			if ((host_item = gethostbyaddr(inaddr, addr_len, family)) == NULL){
				fprintf(stderr, "gethostbyaddr failed.\n");
				exit(5);
			}
		}
		print_host(host_item);
		printf("\n");
	}
	else if (argc == 1) ;
	else{
		fprintf(stderr, "usage: progname [host_name|host_port].\n");
		exit(1);
	}

	sethostent(1); //keep file staying open while using gethost* function
	while ((host_item = gethostent()) != NULL) print_host(host_item);
	endhostent();
}
