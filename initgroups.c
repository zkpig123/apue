//1.add given gid to given user's supplementary group list
//progname should have setuid to run success, be careful
//2.usage: progname given_user_name given_group_id
//3.test result: strange, invoke this progname from shell
//with setuid to root, and then check groups of user 
//given as arg, strange, given new groups id is not in 
//the supplementary group list of the user

#include "assist.c"

int main (int argc, char *argv[])
{
	if (argc != 3){
		fprintf(stderr, "usage: progname given_user_name given_group_id.\n");
		exit(2);
	}
	if (initgroups("zk", 24) == -1){
		perror("initgroups failed.");
		exit(1);
	}
}
