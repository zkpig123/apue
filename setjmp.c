//1.test restore status of variables
//(global, static, register, auto, volatile)
//2.usage: compare result of compilation 
//with -O and without it.then run respectively
//3.conclude: volatile variable hold the value
//while longjmp is called, while auto and register
//variables are restored to the values while calling
//setjmp(without -O compiling option, auto value
//is like volatile, but with -O auto value is optimized
//as register, and register variable is always restored
//to the value whle setjmp is called.

#include "assist.h"

jmp_buf jmp;

void f1 (void)
{
	longjmp(jmp, 1);
}

int global_val;

int main (void)
{
	int autoval;
	register int regval;
	static int static_val;
	volatile int volatile_val;
	autoval = 1;
	regval = 2;
	static_val = 3;
	volatile_val = 4;
	global_val = 5;
	if (setjmp(jmp)){
		printf("\nfrom longjmp.\n");
		printf("autoval:%d, regval:%d, static_val:%d, volatile_val:%d, global_val:%d\n", autoval, regval, static_val, volatile_val, global_val);
	}
	else{
		autoval = 11;
		regval = 22;
		static_val = 33;
		volatile_val = 44;
		global_val = 55;
		printf("after setjmp.\n");
		printf("autoval:%d, regval:%d, static_val:%d, volatile_val:%d, global_val:%d\n", autoval, regval, static_val, volatile_val, global_val);
		f1();
	}

	return 0;
}
