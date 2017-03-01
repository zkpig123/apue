//conclude:on linux, fclose(stdout) not only close the 
//stream, but also close the corresponding file descriptor.

#include "assist.h"

void f1 (void)
{
	pid_t pid, ppid;
	printf("before parent pid:%ld\n", ppid = (long)getpid());
	if ((pid = vfork()) == -1) p_err("fork failed.");
	else if (pid == 0){
	}
	if (pid) printf("after vfork in f1, ppid is %ld\n", ppid);
	printf("after vfork in f1, pid is %ld\n", (long)getpid());
}

void f2 (void)
{
	char buf[200];
	for (int i = 0; i < 200; i++) buf[i] = 0;
	printf("in f2, pid is %ld\n", (long)getpid());
}


int main (void)
{
	f1();
	f2();
}
