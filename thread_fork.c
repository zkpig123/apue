#include "assist.h"

jmp_buf jmp;

void *thread (void *arg)
{
	pid_t pid;
	if ((pid = fork()) == -1) p_err("fork failed.");
	if (pid) return 0;
	int i = 0;
	while (i++ < 5){
		sleep(1);
		printf("pid:%ld, in thread.\n", (long)getpid());
	}
	longjmp(jmp, 1);
}

int main (void)
{
	pthread_t tid;
	if (setjmp(jmp) == 0){
		if (pthread_create(&tid, NULL, thread, (void*)0) == -1) p_err("pthread_create failed.");
		pthread_join(tid, NULL);
	}else{
		printf("pid:%ld, from longjmp.\n", (long)getpid());
	}
	printf("pid:%ld, in main.\n", (long)getpid());
	return 0;
}
