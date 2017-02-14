#include "assist.h"

void err (char *s)
{
	fprintf (stderr, "err:%s\n", s);
	exit (1);
}

void p_err (char *s)
{
	perror (s);
	exit (1);
}
