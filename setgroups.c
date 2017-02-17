//1.add given gid to process's supplementary group list
//progname should have setuid to run success, be careful
//2.usage: progname given_group_id
//3.test result: success.

#include "assist.h"

#ifdef NGROUPS_MAX
#else
#define NGROUPS_MAX 32
#endif

int get_supplementary_groups (gid_t group_list[], int group_list_len)
{
	int groups_num;
	if ((groups_num = getgroups(group_list_len, group_list)) == -1) return -1;
	else return groups_num;
}

void print_supplementary_groups (gid_t group_list[], int groups_list_len)
{
	printf("supplementary group list: ");
	for (int i = 0; i < groups_list_len; i++) printf("%d ", group_list[i]);
	printf("\n");
}

int main (int argc, char *argv[])
{
	if (argc != 2){
		fprintf(stderr, "usage: progname given_group_id.\n");
		exit(2);
	}
	gid_t group_list[NGROUPS_MAX];
	gid_t additional_gid;
	additional_gid = atoll(argv[1]);
	int groups_num;
	printf("real uid:%ld, effective uid:%ld, real gid:%ld, effective gid:%ld\n", getuid(), geteuid(), getgid(), getegid());
	if ((groups_num = get_supplementary_groups(group_list, NGROUPS_MAX)) == -1) p_err("getgroup failed.");
	print_supplementary_groups(group_list, groups_num);
	if (geteuid() == 0){
		//if (initgroups(, 7) == -1) p_err("initgroups err.");
		group_list[groups_num] = additional_gid;
		if (setgroups(groups_num + 1, group_list) == -1) p_err("setgroups.");
	}
	else err("usage: progname should has setuid to run as root.");
	printf("real uid:%ld, effective uid:%ld, real gid:%ld, effective gid:%ld\n", getuid(), geteuid(), getgid(), getegid());

	if ((groups_num = get_supplementary_groups(group_list, NGROUPS_MAX)) == -1) p_err("getgroup failed.");
	print_supplementary_groups(group_list, groups_num);

	return 0;
}
