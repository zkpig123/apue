//1.usage: progname another_user_name,
//2.program need saved id bit to be set,
//should run from non-root user,and give a
//user name different from runner as arg to 
//program
//3.conclude:root process could setreuid to 
//any combination of uid in the system

#include "assist.h"
extern int errno;

int main (int argc, char *argv[])
{
	if (argc != 3) err("usage: progname another_user_name.");
	struct passwd *pw_entry, pw_entry1, pw_entry2;
	errno = 0;
	printf("read uid is %ld, effective uid is %ld\n", (long int)getuid(), (long int)geteuid());
	if ((pw_entry = getpwnam(argv[1])) == NULL){
		if (errno) p_err("getpwnam failed.");
		err("invalid user name.");
	}
	memcpy(&pw_entry1, pw_entry, sizeof(struct passwd));
	errno = 0;
	if ((pw_entry = getpwnam(argv[2])) == NULL){
		if (errno) p_err("getpwnam failed.");
		err("invalid user name.");
	}
	memcpy(&pw_entry2, pw_entry, sizeof(struct passwd));
	printf("uid of given user1 %s is %ld.\n", argv[1], pw_entry1.pw_uid);
	printf("uid of given user2 %s is %ld.\n", argv[2], pw_entry2.pw_uid);
	if (setreuid(pw_entry1.pw_uid, pw_entry2.pw_uid) == -1){
		fprintf(stderr, "setreuid(%s, %s) from effective id:%ld failed.\n", argv[1], argv[2], (long)geteuid());
		return 1;
	}else{
		printf("setuid(%s, %s) whose id is %ld and %ld from effective id:%ld success.\n", argv[1], argv[2], (long)pw_entry1.pw_uid, (long)pw_entry2.pw_uid, (long)geteuid());
		printf("now real uid:%ld, effective uid:%ld.\n", getuid(), geteuid());
		return 0;
	}
}
