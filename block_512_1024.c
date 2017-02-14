//check existence of macro POSIXLY_CORRECT, if 
//defined, then block size of st_blocks from stat
//system invocation is sized 1024, if not, 512.

#include "assist.h"

int main (void)
{
#ifdef POSIXLY_CORRECT
	printf ("POSIXLY_CORRECT is %d\n.", POSIXLY_CORRECT);
#else
	printf ("POSIXLY_CORRECT is not defined.\n");
#endif
}
