#include "assist.h"

void err (const char *s)
{
	fprintf(stderr, "err:%s\n", s);
	exit (1);
}

void p_err (const char *s)
{
	perror(s);
	exit (1);
}

void swap_byte (unsigned char *c1, unsigned char *c2)
{
	*c1 = *c1 ^ *c2;
	*c2 = *c1 ^ *c2;
	*c1 = *c1 ^ *c2;
}

int network_to_host (int bytes)
{
	short s = 1;
	unsigned char *a, *b;
	unsigned char *num = (unsigned char*)&bytes;
	if (*(char*)&s == 1){ //little endian
		a = num;
		b = &num[3];
		swap_byte(a, b);
		a = &num[1];
		b = &num[2];
		swap_byte(a, b);
		return bytes;
	}
	else return bytes;
}

int vm (void *pos, int bytes)
{
	int i;
	unsigned char *ch;
	if (pos == NULL)
	{
		printf ("in func vm, err: NULL pointer\n");
		return -1;
	}
	if (bytes == 0)
	{
		printf ("0 bytes to view\n");
		return -1;
	}

	ch = (unsigned char*) pos;
	printf ("binary view is\n");
	for (i = 0; i <= bytes - 1; i++)
	{
		int k;
		for (k = 7; k >= 0; k--)
		{
			printf ("%u", (ch[i] & ( 1 << k)) >> k);
		}
		printf (" ");
	}
	printf ("\n");

	return 0;
}
