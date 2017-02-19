#include "assist.h"

int put_env (char *s)
{
	if (putenv(s)) return 1;
	else return 0;
}

int set_env (char *name, char *value, int rewrite)
{
	if (setenv(name, value, rewrite) == -1) return 1;
	else return 0;
}

int main (int argc, char *argv[])
{
	char str1[] = "put=env";
	char name[] = "set";
	char value[] = "env";
	if (put_env(str1)) fprintf(stderr, "put_env failed.\n");
	else printf("put=%s\n", getenv("put"));
	if (set_env(name, value, 1)) fprintf(stderr, "set_env failed.\n");
	else printf("set=%s\n", getenv("set"));

	return 0;
}
