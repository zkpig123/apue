//1.test privileges required to change
//access or modify time of a file
//2.usage: progname file_name_to_test

#include "assist.h"

int main (int argc, char *argv[])
{
	if (argc != 2) err ("usage: progname file_name_to_test.");
	struct stat file_status;
	if (stat (argv[1], &file_status) == -1) p_err ("stat failed.");
	printf ("process real uid:%ld, euid:%ld, real gid:%ld, egid:%ld\n", getuid(), geteuid(), getgid(), getegid());
	printf ("file uid:%ld, gid:%ld\n", file_status.st_uid, file_status.st_gid);
	printf ("file access attribute:");
	if (file_status.st_mode & S_IWUSR) printf ("user_write ");
	if (file_status.st_mode & S_IWGRP) printf ("group_write ");
	if (file_status.st_mode & S_IWOTH) printf ("other_write ");

	printf ("set atime and mtime to specified time.\n");
	struct timespec inode_time[2];
	inode_time[0].tv_sec = 10;
	inode_time[0].tv_nsec = 0;
	inode_time[1].tv_sec = 20;
	inode_time[1].tv_nsec = 0;
	if (utimensat (AT_FDCWD, argv[1], inode_time, 0) == -1) perror ("utimensat failed.");
	else printf ("success.\n");
	printf ("press anykey to continue..\n");
	getchar ();

	printf ("set atime and mtime to current time.\n");
	inode_time[0].tv_nsec = UTIME_NOW;
	inode_time[1].tv_nsec = UTIME_NOW;
	if (utimensat (AT_FDCWD, argv[1], inode_time, 0) == -1) perror ("utimensat failed.");
	else printf ("success.\n");
}
