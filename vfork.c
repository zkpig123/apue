#include "assist.h"

int global = 1;

int main (void)
{
	int var;
	pid_t pid;

	var = 20;
	printf("before vfork.\n");
	if ((pid = vfork()) < 0){
		err("vfork error");
	} else if (pid == 0){
		global++;
		var++;
		_exit(0);
	}
	printf("pid = %ld, global = %d, var = %d\n", (long)getpid(), global, var);
	exit(0);
}
