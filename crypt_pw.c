#include <unistd.h>
#include <crypt.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
	if (argc != 3){
		fprintf(stderr, "usage: progname salt string_to_encrypt.\n");
		exit(1);
	}
	printf("string:%s\n", argv[2]);
	printf("salt:%s\n", argv[1]);
	char *s = crypt(argv[1], argv[2]);
	if (s != NULL) printf("encrypted data:%s\n", s);
	else fprintf(stderr, "crypt failed.\n");

	return 0;
}
