//note execl need first arg to be full path, but
//usage here succeed, kernel add "testinterp" to
//full path??

#include "assist.h"

int main(void)
{
	pid_t pid;
	if ((pid = fork()) < 0) {
		err("fork error");
	} else if (pid == 0) {
		/* child */
		if (execl("testinterp", "testinterp", "myarg1", "MY ARG2", (char *)0) < 0) err("execl error");
	}
	if (waitpid(pid, NULL, 0) < 0) /* parent */
	err("waitpid error");
	exit(0);
}
