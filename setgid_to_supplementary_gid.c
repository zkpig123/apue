//1.test whether process can change gid of file if 
//one of process's supplementary gid equal the gid
//of file.
//2.usage : progname group_name_to_change_to
//3.conclude:unable

#include "assist.h"

int main (int argc, char *argv[])
{
	struct stat file_status;
	struct group *group_info;
	gid_t target_gid;

	if (argc != 2) err ("usage : progname group_name_to_set_egid_to.");
	if ((group_info = getgrnam (argv[1])) == NULL) p_err ("getgrnam failed.");	
	printf ("current real gid is %ld, effective id is %ld\n", getgid (), getegid ());
	if (setregid (-1, group_info->gr_gid) == -1) fprintf (stderr, "set egid failed.");
	if (setregid (group_info->gr_gid, -1) == -1) fprintf (stderr, "set real gid failed.");
	printf ("now real gid is %ld, effective id is %ld\n", getgid (), getegid ());
}
