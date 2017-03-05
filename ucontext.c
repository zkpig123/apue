#include <stdio.h>  
#include <ucontext.h>  
#include <unistd.h>  
  
int main(int argc, const char *argv[])
{  
	int i = 0;
	ucontext_t context;  

	getcontext(&context);  
	printf("Hello world %d\n", i++);  
	sleep(1);  
	setcontext(&context);  
	return 0;  

}  
