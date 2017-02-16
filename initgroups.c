//1.add given gid to given user's supplementary group list
//progname should have setuid to run success, be careful
//2.usage: progname given_user_name given_group_id
//3.if name passed to initgroup func differ from real uid
//of process, then only group which is given as arg to 
//initgroup and groups that real user of process and user
//whose name is passed to inigroup are both in are added to
//supplementary groups of calling process

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
	if (argc != 3){
		fprintf(stderr, "usage: progname user_name_to_set given_group_id.\n");
		exit(2);
	}
	gid_t group_list[NGROUPS_MAX];
	gid_t additional_gid;
	additional_gid = atoll(argv[2]);
	int groups_num;
	printf("real uid:%ld, effective uid:%ld, real gid:%ld, effective gid:%ld\n", getuid(), geteuid(), getgid(), getegid());
	if ((groups_num = get_supplementary_groups(group_list, NGROUPS_MAX)) == -1) p_err("getgroup failed.");
	print_supplementary_groups(group_list, groups_num);
	if (geteuid() == 0){
		if (initgroups(argv[1], additional_gid) == -1) p_err("setgroups err.");
		else printf("initgroups success.\n");
	}
	else err("usage: progname should has setuid to run as root.");
	printf("real uid:%ld, effective uid:%ld, real gid:%ld, effective gid:%ld\n", getuid(), geteuid(), getgid(), getegid());

	if ((groups_num = get_supplementary_groups(group_list, NGROUPS_MAX)) == -1) p_err("getgroup failed.");
	print_supplementary_groups(group_list, groups_num);

	return 0;
}
