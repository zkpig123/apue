#include <stdio.h>
#include <limits.h>

#define BUFLEN 10

int main(void)
{
	FILE *fp;
	char *buf;
	size_t size;
	fp = open_memstream(&buf, &size);
	if (fp == NULL){
		fprintf(stderr, "open_memstream failed.");
		return 1;
	}
	fprintf(fp, "ab");
	fflush(fp);
	printf ("buf addr:%p\n", buf);
	for(int i = 0; i <= SHRT_MAX; i++) fprintf(fp, "ab");
	fflush(fp);
	printf ("now buf addr:%p\n", buf);
}
