#include "assist.h"

void sig_handler (int signo)
{

}

void *thread (void *arg)
{
	if ((int)(long)arg == 1) alarm(3);
}

int main (void)
{
}
