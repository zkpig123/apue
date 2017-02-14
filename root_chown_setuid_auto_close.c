//1.test whether setgid and setuid are cleared
//when super user call chown on a file.(non 
//super user can't call chown on linux)
//2.usage: progname file_name_to_test user_name_to_set_file_owner
//3.program should run as super user or set uid
//4.conclusion:setuid is cleared out even though chown is run
//as super user/root, but setgid is not

#include "assist.h"

int main (int argc, char *argv[])
{

	struct passwd *password_file_item;
	struct stat file_status;
	if (argc != 3) err ("usage: progname file_name_to_test user_name_to_set_file_owner.");
	if ((password_file_item = getpwnam (argv[2])) == NULL) p_err ("getpwnam failed.");
	if (chmod (argv[1], S_ISUID | S_ISGID | S_IRUSR | S_IWUSR | S_IRGRP) == -1) p_err ("chmod failed.");
	if (stat (argv[1], &file_status) == -1) p_err ("stat failed.");
	printf ("current file uid is %ld, gid is %ld\n", file_status.st_uid, file_status.st_gid);
	printf ("setid status:");
	if (file_status.st_mode & S_ISUID) printf ("setuid ");
	if (file_status.st_mode & S_ISGID) printf ("setgid ");
	printf ("\n\n");
	printf ("type any key to continue.\n\n");
	getchar ();

	if (chown (argv[1], password_file_item->pw_uid, password_file_item->pw_gid)) p_err ("chown failed.");
	printf ("chown and file uid is %ld, gid is %ld\n", file_status.st_uid, file_status.st_gid);
	if (stat (argv[1], &file_status) == -1) p_err ("stat failed.");
	printf ("current file uid is %ld, gid is %ld\n", file_status.st_uid, file_status.st_gid);
	printf ("setid status:");
	if (file_status.st_mode & S_ISUID) printf ("setuid ");
	if (file_status.st_mode & S_ISGID) printf ("setgid ");

	return 0;
}
