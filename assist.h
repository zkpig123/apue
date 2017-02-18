#ifndef _ASSIST_H_
#define _ASSIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <grp.h>
#include <errno.h>
#include <netdb.h>
#include <sys/utsname.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <time.h>

void err (const char *s);
void p_err (const char *s);

#endif
