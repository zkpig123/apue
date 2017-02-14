//1.list size and blocks of file in current directory.

#include "assist.h"

int main (void)
{
	struct dirent *dir_item;
	DIR *cur_dir;
	struct stat file_stat;

	if ((cur_dir = opendir (".")) == NULL) p_err ("opendir failed.");
	printf ("initial position in DIR is %ld\n", telldir (cur_dir));
	while ((dir_item = readdir (cur_dir)) != NULL){
		printf ("name:%s\n", dir_item->d_name);
		if (stat (dir_item->d_name, &file_stat) != -1){
			printf ("file size: %ldbytes \n", file_stat.st_size);
			printf ("file block size: %ld bytes\n", file_stat.st_blksize);
			printf ("file blocks: %ld\n", file_stat.st_blocks);
			printf ("\n");
		}
		else printf ("stat err.\n\n");
	}
	closedir (cur_dir);
}
