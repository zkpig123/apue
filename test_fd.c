#include "assist.h"

#define TEST_FILE "test_fd.testfile"

int main (void)
{
	int fd, dup_fd, newfd;

	if (setvbuf(stdout, NULL, _IONBF, 0)) err("setvbuf failed.");
	dup_fd = 38;
	if ((fd = open(TEST_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");
	if (dup2(fd, dup_fd) == -1) p_err("dup2 failed.");
	if ((newfd = open(TEST_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1) p_err("open failed.");
	if (write(fd, "X", 1) != 1) p_err("write failed.");
	printf("open fd on %s, dup2 fd to newfd, and open again %s on newfd.\n", TEST_FILE, TEST_FILE);
	printf("after write a byte using fd, now pos of three descriptor is.\n");
	off_t pos_fd, pos_dup_fd, pos_newfd;
	if ((pos_fd = lseek(fd, 0, SEEK_CUR)) == -1) p_err("lseek on fd failed.\n");
	if ((pos_dup_fd = lseek(dup_fd, 0, SEEK_CUR)) == -1) p_err("lseek on dup_fd failed.\n");
	if ((pos_newfd = lseek(newfd, 0, SEEK_CUR)) == -1) p_err("lseek on dup_newfd failed.\n");
	printf("pos_fd:%ld, pos_dup_fd:%ld, pos_newfd:%ld\n", pos_fd, pos_dup_fd, pos_newfd);
	if (close(fd) == -1) p_err("close fd failed.");
	if (write(dup_fd, "X", 1) != 1) fprintf(stderr, "write dup_fd after closing fd failed.");
	else printf("write dup_fd after closing fd success.\n");
	if (lseek(newfd, 5, SEEK_SET) == -1) p_err("lseek on newfd failed.");
	if (write(newfd, "X", 1) != 1) fprintf(stderr, "write newfd failed.");
	else printf("write newfd success.\n");

	return 0;
}
