//1.usage: progname uid
//2.return highest priority of nice value of given real user id

#include "assist.h"

int main (int argc, char *argv[])
{
	if (argc != 2) err("usage: progname uid.");
	for (int i = 0; argv[1][i] != '\0'; i++){
		if (!isdigit(argv[1][i])) err("invalid user id.");
	}

	int nzero;
#ifdef NZERO
	nzero = NZERO;
#elif defined(_SC_NZERO)
	errno = 0;
	if ((nzero = sysconf(_SC_NZERO)) == -1 && errno) p_err("sysconf to get NZERO failed.");
#else
#error NZERO undefined
#endif

	int nice_value;
	uid_t uid;
	uid = (uid_t)atoll(argv[1]);
	errno = 0;
	nice_value = getpriority(PRIO_USER, (int)uid);
	if (errno) p_err("getpriority failed.");
	printf("highest priority of user whose id is %ld is %d\n", (long)uid, (int)(nice_value + nzero));

	return 0;
}
