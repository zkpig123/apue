//1.given protocol name or protocol number,
//get and print protocol entry from file
// /etc/protocols, and then print all entries
//in that file
//2.usage: progname protocol_name|protocol_number

#include "assist.h"

void print_protocol_entry (struct protoent *protocol_item)
{
	printf("protocol name:%s\n", protocol_item->p_name);
	printf("alias list:\n");
	for (char **list = protocol_item->p_aliases; *list != NULL; list++) printf("           %s\n", *list);
	printf("protocol number:%d\n", protocol_item->p_proto);
}

int main (int argc, char *argv[])
{
	if (argc != 2) err("usage: progname protocol_name|protocol_number.");
	struct protoent *protocol_item;
	if (isdigit(argv[1][0])){
		int protocol_number;
		if ((protocol_number = atoi(argv[1])) < 0) err("invalid portocol number.");
		if ((protocol_item = getprotobynumber(protocol_number)) == NULL) err("getprotobynumber failed.");
	}
	else{
		if ((protocol_item = getprotobyname(argv[1])) == NULL) err("getprotobyname failed.");
	}
	print_protocol_entry(protocol_item);
	printf("\n");

	setprotoent(1); //let file stay open while using getproto* function
	while ((protocol_item = getprotoent()) != NULL) print_protocol_entry(protocol_item);
	endprotoent();

	return 0;
}
