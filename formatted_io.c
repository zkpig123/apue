#include "assist.h"

int main (void)
{
	int num = 134;
	char *p = (char *)&num;
	printf ("%%+#d %+#d\n", num);
	printf ("%%+#o %+#o\n", num);
	printf ("%%o %o\n", num);
	printf ("%%+#x %+#x\n", p);
	printf ("%%x %x\n", p);
	printf ("%%p %p\n", p);
	printf ("%%tx %tx\n", p);
	printf ("%%#tx %#tx\n", p);
	char s[] = "abcde";
	printf ("%%10.3s %10.3s\n", s);
	printf ("num:%zd\n", num);
	double b = 3.538838;
	printf ("%%Lf %lf\n", b);
	printf ("%%jd %jd\n", num);
	printf ("%%i %i\n", num);
	num = -num;
	printf ("%%o %o\n", num);
	printf ("%%a %a\n", b);
	int chars_written;
	printf ("%%n %n\n", &chars_written);
	printf ("chars written:%d\n", chars_written);
	printf ("%%2$f %%1$d %2$f %1$d\n", num, b);
	printf ("%%*2$f %%1$d %2$f %1$d\n", num, b);
}
