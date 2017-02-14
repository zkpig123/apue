//usage: progname file_name

#include "assist.h"

int main (int argc, char *argv[])
{
	if (argc != 2) err ("usage: progname file_name.");
	struct stat file_status;
	int fd;
	if (stat (argv[1], &file_status) == -1) p_err ("stat failed.");
	if ((fd = open (argv[1], O_WRONLY)) == -1) p_err ("open failed.");
	else printf ("euid:%ld, file owner uid:%ld, open %s successful.\n", geteuid (), file_status.st_uid, argv[1]);
	if (write (fd, "hi", 2) == -1) p_err ("write failed.");

	return 0;
}
