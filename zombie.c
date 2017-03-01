#include "assist.h"

int main (void)
{
	pid_t pid;
	if ((pid = vfork()) == -1) p_err("fork err");
	if (pid == 0) return 0;
	printf("child's pid is %ld, run ps aux|grep %d to see the zombie state.\n", pid, pid);
	while (1){
	}
}
