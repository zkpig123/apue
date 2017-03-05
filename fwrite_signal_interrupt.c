//alarm to interrupt fwrite which is writing 
//a big file, to see whether fwrite failed.
//
//conclude:sigalrm is blocked when fwrite is executed,
//after fwrite it's unblocked

#include "assist.h"

void sig_handler (int signo)
{
	printf("signal %ld catched.\n", signo);
}

int main (void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if(sigaction(SIGALRM, &act, NULL) == -1) p_err("sigaction 4 SIGALRM failed.");

	int fd;
	if ((fd = open("fwrite_signal_interrupt.test", O_CREAT | O_RDWR | O_SYNC, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");
	FILE *fp;
	fp = fdopen(fd, "a+");
	if (fp == NULL) p_err("fopen failed.");
	char *buf;
	if ((buf = calloc(1024L * 1024 * 1024, 1)) == NULL) err("malloc failed.");
	alarm(2);
	if (fwrite(buf, 1024L * 1024 * 1024, 1, fp) != 1){
		free(buf);
		fprintf(stderr, "fwrite failed.");
		return 1;
	}
	free(buf);
	if (fclose(fp) == EOF) p_err("fclose failed.");
	if ((fd = open("fwrite_signal_interrupt.test", O_RDONLY)) == -1) p_err("open failed.");
	size_t bytes, total_bytes;
	char buffer[BUFSIZ];
	total_bytes = 0;
	while ((bytes = read(fd, buffer, BUFSIZ)) > 0){
		for (size_t i = 0; i < bytes; i++){
			if (buffer[i] != 0){
				printf("fwrite failed at %zu bytes!\n", total_bytes + i + 1);
			}
		}
		total_bytes += bytes;
	}
	if (bytes == -1) p_err("read failed.");

	return 0;
}
