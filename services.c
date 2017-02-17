//1.given service name or service port,
//get and print service entry from file
// /etc/services, and then print all entries
//in that file
//2.usage: progname service_name|service_port

#include "assist.h"


void pr_services (struct servent *service_ent)
{
	printf("serv name:%s\n", service_ent->s_name);
	printf("alias list:\n");
	for (char **list = service_ent->s_aliases; *list != NULL; list++){
		printf("           %s\n", *list);
	}
	printf("port number:%hu\n", ntohs(service_ent->s_port)); //should convert port from network order to host order
	printf("protocol:%s\n", service_ent->s_proto);
}

int main (int argc, char *argv[])
{
	if (argc != 2) err("usage: progname service_name|service_port\n");
	struct servent *service_ent;
	if (isdigit(argv[1][0])){
		int port;
		if ((port = atoi(argv[1])) < 0 || port > 65535){
			err("invalid port number.\n");
		}
		if ((service_ent = getservbyport(port, NULL)) == NULL) err("getservbyport failed."); //NULL to match any protocol
	}
	else{
		if ((service_ent = getservbyname(argv[1], NULL)) == NULL) err("getservbyname failed.");
	}
	pr_services(service_ent);
	printf("\n");
	setservent(1); //1 to let file stay open while using getserv* func
	while ((service_ent = getservent()) != NULL) pr_services(service_ent);
	endservent();

	return 0;
}
