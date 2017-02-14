//1.test whether setgid and setuid are cleared
//when user write to file
//2.usage: progname file_name_to_test
//3.conclude:when non root user write to a file
//belong to another user, then if this file has
//setid, then setid are all cleared out, but if
//writter is root, then setid remain as original

#include "assist.h"

int main (int argc, char *argv[])
{

	struct stat file_status;
	if (argc != 2) err ("usage: progname file_name_to_test.");
	if (stat (argv[1], &file_status) == -1) p_err ("stat failed.");
	printf ("current file setid status:");
	if (file_status.st_mode & S_ISUID) printf ("setuid ");
	if (file_status.st_mode & S_ISGID) printf ("setgid ");
	printf ("\n\n");
	printf ("type any key to continue.\n\n");
	getchar ();

	int fd;
	if ((fd = open (argv[1], O_WRONLY)) == -1) p_err ("open failed.");
	if (write (fd, " ", 1) < 0) p_err ("write failed.");
	if (close (fd)) p_err ("close failed.");
	if (stat (argv[1], &file_status) == -1) p_err ("stat failed.");
	printf ("current file setid status:");
	if (file_status.st_mode & S_ISUID) printf ("setuid ");
	if (file_status.st_mode & S_ISGID) printf ("setgid ");

	return 0;
}
