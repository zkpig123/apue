#include <stdio.h>

#define BUFLEN 10

int main(void)
{
	FILE *fp;
	char buf[BUFLEN];
	fp = NULL;
	fp = fmemopen(buf, BUFLEN, "r+");
	if (fp == NULL){
		fprintf(stderr, "fmemopen failed.\n");
		return 1;
	}
	fprintf(fp, "abcdefghijkl");
	fclose(fp);
	printf("buf content: ");
	for(int i = 0; i < BUFLEN; i++) printf("%c", buf[i]);
	printf("\n");
}
