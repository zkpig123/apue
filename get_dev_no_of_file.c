//usage: progname file_path

#include "assist.h"

int main (int argc, char *argv[])
{
	if (argc != 2) err ("usage: progname file_path");
	struct stat file_status;
	if (stat (argv[1], &file_status) == -1) p_err ("stat failed.");
	printf ("file:%s\n");
	printf ("st_dev : major_dev_no:%lu, minor_dev_no:%lu\n", major (file_status.st_dev), minor (file_status.st_dev));
	printf ("st_rdev : major_dev_no:%lu, minor_dev_no:%lu\n", major (file_status.st_rdev), minor (file_status.st_rdev));

	return 0;
}
