//func in this source file are not in POSIX.1!

#include <utmp.h>
#include <stdio.h>

int main (void)
{
	struct utmp *item;
	setutent();
	item = getutent();
	endutent();
	printf ("ut_type:%hd\nut-pid:%ld\nut_line:%s\nut_id:%s\nut_user:%s\nut_host:%s\n", item->ut_type, item->ut_pid, item->ut_line, item->ut_id, item->ut_user, item->ut_host);
}
