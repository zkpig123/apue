//Example of non thread-safe func getenv
//If two threads call it at the same time,
//they will see inconsistent results, because 
//the string returned is stored in a single 
//static buffer that is shared by all threads
//calling getenv.

#include "assist.h"

#define MAXSTRINGSZ 4096
static char envbuf[MAXSTRINGSZ];
extern char **environ;
char * getenv(const char *name)
{
	int i, len;
	len = strlen(name);
	for (i = 0; environ[i] != NULL; i++) {
		if ((strncmp(name, environ[i], len) == 0) &&
		(environ[i][len] == '=')) {
			strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ-1);
			return(envbuf);
		}
	}

	return(NULL);
}

void *thread (void *arg)
{
	char *en;
	long int value;
	value = (long int)arg;
	en = getenv("PATH");
	if (value == 1) sprintf(en, "hihi.");
	printf("getenv: %s\n", en);
}

int main (void)
{
	pthread_t thread_id1, thread_id2, thread_id3;
	pthread_create(&thread_id1, NULL, thread, (void*)0);
	pthread_create(&thread_id2, NULL, thread, (void*)1);
	pthread_create(&thread_id3, NULL, thread, (void*)0);
	pthread_join(thread_id1, NULL);
	pthread_join(thread_id1, NULL);
	pthread_join(thread_id1, NULL);

	return 0;
}
