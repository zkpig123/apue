//1.usage: progname another_user_name,
//2.program need saved id bit to be set,
//should run from non-root user,and give a
//user name different from runner as arg to 
//program
//3.conclude:root process could setuid to 
//any uid in the system, not just from uid
//of whom is now running the program

#include "assist.h"
extern int errno;

int main (int argc, char *argv[])
{
	if (argc != 2) err("usage: progname another_user_name.");
	struct passwd *pw_entry;
	errno = 0;
	printf("read uid is %ld, effective uid is %ld\n", (long int)getuid(), (long int)geteuid());
	if ((pw_entry = getpwnam(argv[1])) == NULL){
		if (errno) p_err("getpwnam failed.");
		err("invalid user name.");
	}
	printf("uid of given user %s is %ld.\n", argv[1], pw_entry->pw_uid);
	if (setuid(pw_entry->pw_uid) == -1){
		fprintf(stderr, "setuid(%s) from effective id:%ld failed.\n", argv[1], (long)geteuid());
		return 1;
	}else{
		printf("setuid(%s) whose id is %ld from effective id:%ld success.\n", argv[1], (long)pw_entry->pw_uid, (long)geteuid());
		printf("now real uid:%ld, effective uid:%ld.\n", getuid(), geteuid());
		return 0;
	}
}
