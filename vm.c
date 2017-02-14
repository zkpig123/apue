#include <stdio.h>

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
