//POSIX define utmpx.h

#include <utmpx.h>
#include <stdio.h>
#include <stdlib.h>

int main (void)
{
	struct utmpx *u = NULL;
	setutxent();
	u = getutxent();
	if (u == NULL){
		perror("getutxent failed.");
		exit(1);
	}
	endutxent();
	printf("type:%hd\n", u->ut_type);
	printf("pid:%ld\n", u->ut_pid);
	printf("id:");
	for (int i = 0; i < 4; i++) printf("%c", u->ut_id[i]);
	printf("\n");
	printf("device:%s\n", u->ut_line);
	printf("user:%s\n", u->ut_user);
	printf("host:%s\n", u->ut_host);

	return 0;
}
