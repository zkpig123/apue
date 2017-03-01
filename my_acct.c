#include "assist.h"

#define DEFAULT_ACCOUNTING_FILE "/var/log/account/pacct"

int main (int argc, char *argv[])
{
#ifndef NAME_MAX
#define NAME_MAX 256
#endif
	char *progname;
	if (argc != 2) err("usage: progname on|off|ACCOUNTING_FILE.");
	errno = 0;
	if (strcmp("on", argv[1]) == 0){
		if (acct(DEFAULT_ACCOUNTING_FILE) == -1) perror("Turning on process accounting failed: ");
		else{
			printf("Turning on process accounting, file set to the default '/var/log/account/pacct'.\n");
			return 0;
		}
	}else if (strcmp("off", argv[1]) == 0){
		if (acct(NULL) == -1){
			perror("Turning off process accounting failed: ");
		}else{
			printf("Turning off process accounting.\n");
			return 0;
		}
	}else{
		if (acct(argv[1]) == -1){
			perror("Turning on process accounting failed: ");
		}else{
			printf("Turning on process accounting, file set to '%s'.\n", argv[1]);
			return 0;
		}
	}

	return 0;
}
