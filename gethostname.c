#include "assist.h"

int main (void)
{
	char *hostname;
	long name_max;
#ifdef HOST_NAME_MAX
	name_max = HOST_NAME_MAX;
#else
	#ifdef _SC_HOST_NAME_MAX
	name_max = sysconf(_SC_HOST_NAME_MAX);
	#else
	#define GUESS_MAX 256 + 1
	name_max = GUESS_MAX;
	#endif
#endif
	if ((hostname = malloc(name_max)) == NULL) err("malloc failed.");
	if (gethostname(hostname, name_max) == -1) p_err("gethostname failed.");
	printf("hostname:%s\n", hostname);

	return 0;
}
