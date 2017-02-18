#include "assist.h"

int main (void)
{
	struct utsname info;
	if (uname(&info) == -1) p_err("uname failed.");
	printf("sysname:%s\n", info.sysname);
	printf("nodename:%s\n", info.nodename);
	printf("release:%s\n", info.release);
	printf("version:%s\n", info.version);
	printf("machine:%s\n", info.machine);

	return 0;
}
