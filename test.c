#include "assist.h"

int main (void)
{
	if (close(4) == -1) p_err("close failed.\n");

	return 0;
}
