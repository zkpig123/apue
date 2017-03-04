//get segment fault probably because getpwnam
//internally called free: getpwnam in main was
//interrupted while it is in the middle of free
//,for example, update the list of memory area
//using by malloc, then in the interrupt func,
//getpwnam free again, then it's a havoc

#include "assist.h"

static void my_alarm(int signo)
{
	struct passwd *rootptr;
	printf("in signal handler\n");
	if ((rootptr = getpwnam("root")) == NULL) p_err("getpwnam(root) error");
	alarm(1);
}
int main(void)
{
	struct passwd *ptr;
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = my_alarm;
	if (sigaction(SIGALRM, &act, NULL) == -1) p_err("sigaction failed.");
	alarm(1);
	for ( ; ; ) {
	if ((ptr = getpwnam("sar")) == NULL)
	printf("getpwnam error");
	if (strcmp(ptr->pw_name, "sar") != 0)
	printf("return value corrupted!, pw_name = %s\n",
	ptr->pw_name);
}
}
