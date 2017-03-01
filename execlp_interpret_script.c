#include "assist.h"
//execlp may failed if PATH doesn't contain
//current directory, as execlp search the
//current working path!

int main(void)
{
	long name_max;
#ifdef NAME_MAX
	name_max = NAME_MAX;
#elif defined(_PC_NAME_MAX)
	name_max = pathconf(".", _PC_NAME_MAX);
#else
	name_max = 256;
#endif
	pid_t pid;
	char *buf;
	if ((buf = malloc(name_max)) == NULL) err("malloc failed.");
	if (getcwd(buf, name_max) == NULL) p_err("getcwd failed.");
	printf("cwd is %s.\n", buf);
	if ((pid = fork()) < 0) {
		err("fork error");
	} else if (pid == 0) {
		/* child */
		if (execlp("testinterp", "testinterp", "myarg1", "MY ARG2", (char *)0) < 0) p_err("execl error");
	}
	if (waitpid(pid, NULL, 0) < 0) /* parent */
	err("waitpid error");
	exit(0);
}
