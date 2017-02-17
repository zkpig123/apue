//1.given network name or network addr,
//get and print network entry from file
// /etc/networks, and then print all entries
//in that file
//2.usage: progname network_name|network_addr

#include "assist.h"

int get_addr_from_string (char *addr_str, uint32_t *addr)
{
	struct in_addr inaddr;
	if (inet_aton(addr_str, &inaddr) == 0) return 1;
	*addr = ntohl((uint32_t)inaddr.s_addr);
	return 0;
}

void pr_network_item (struct netent *network_item)
{
	printf("network:\n");
	printf("name:%s\n", network_item->n_name);
	printf("alias list:\n");
	for (char **list = network_item->n_aliases; *list != NULL; list++){
		printf("           %s\n", *list);
	}
	printf("address type:%d\n", network_item->n_addrtype);
	printf("network number:%d\n", network_item->n_net);
}

int main (int argc, char *argv[])
{
	if (argc != 2){
		fprintf(stderr, "usage: progname network_addr.\n");
		exit(1);
	}
	struct netent *network_item;
	if (isdigit(argv[1][0])){
		uint32_t addr;
		if (get_addr_from_string(argv[1], &addr)){
			fprintf(stderr, "invalid addr.\n");
			exit(2);
		}
		if ((network_item = getnetbyaddr(addr, AF_INET)) == NULL){
			fprintf(stderr, "getnetbyaddr failed.\n");
			exit(3);
		}
	}
	else{
		if ((network_item = getnetbyname(argv[1])) == NULL){
			fprintf(stderr, "getnetbyname failed.\n");
			exit(4);
		}
	}

	pr_network_item (network_item);
	printf("\n");
	printf("now traverse through /etc/networks\n");
	setnetent(1); //non zero arg to let file stay open while using getnet* function
	while ((network_item = getnetent()) != NULL) pr_network_item (network_item);
	endnetent();
}
