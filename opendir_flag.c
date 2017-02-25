#include "assist.h"

extern int errno;

int main(void)
{
	DIR *dirent;
	if ((dirent = opendir(".")) == NULL) p_err("opendir failed.");
	int fd;
	errno = 0;
	if ((fd = dirfd(dirent)) == -1) p_err("dirfd failed.");
	if (fcntl(fd, F_GETFD) & FD_CLOEXEC){
		printf("CLOEXEC is set for file descriptor from opendir.\n");
	}
	else printf("CLOEXEC is not set for file descriptor from opendir.\n");
}
