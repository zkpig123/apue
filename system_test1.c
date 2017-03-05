//usage: progname command_to_run
//conclude: return value of system is 
//status of shell that was invoked
//by the system function, not the
//status of the process that was invoked
//by through exec!

#include "assist.h"

int main (int argc, char *argv[])
{
	if (argc != 2) err("usage: progname command_to_run.");
	printf("status of system(%s) is %d\n", argv[1], system(argv[1]));

	return 0;
}
