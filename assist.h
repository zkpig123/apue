#ifndef _ASSIST_H_
#define _ASSIST_H_

#include <stdio.h>
#include <dirent.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <netdb.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/resource.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <poll.h>

void t_err (const char *s, int err_no);
void err (const char *s);
void p_err (const char *s);
void swap_byte (unsigned char *c1, unsigned char *c2);
int network_to_host (int bytes);
int vm (void *pos, int bytes);
#endif
