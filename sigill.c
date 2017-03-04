#include "assist.h"

void sig_handler (int signo)
{
	printf("signal %ld catched.\n", signo);
	exit(0);
}

typedef void(*FUNC)(void);

int main(void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if(sigaction(SIGILL, &act, NULL) == -1) p_err("sigaction 4 SIGILL failed.");
	if(sigaction(SIGSEGV, &act, NULL) == -1) p_err("sigaction 4 SIGSEGV failed.");
	/*FUNC fp = NULL;
	fp();*/ //this generate a SIGSEGV
    //const static unsigned char insn[4] = { 0xff, 0xff, 0xff, 0xff }; //this generate a SIGSEGV
    const static unsigned char insn[4] = { 0x01, 0x02, 0x03, 0x04 };
    FUNC function = (FUNC) insn;
    function();
}
