//my version of apue 12.2 putenv func
//usage: progname VAR1=XXX VAR2=XXX ... .

#include "assist.h"

#define INVALID_ENVIRONMENT_VARIABLE 1
#define PTHREAD_ONCE_FAILED 2
#define LOCK_MUTEX_FAILED 3
#define ALLOC_MEMORY_FAILED 4
#define DEFAULT_ENVIRON_ITEMS 64

static pthread_mutex_t mutex;
static pthread_once_t once;
extern char **environ;
static int has_environ_malloced;
size_t cur_items;
size_t max_items;

void *block_signal (sigset_t *oldmask)
{
	sigset_t newmask;
	sigfillset(&newmask);
	pthread_sigmask(SIG_BLOCK, &newmask, oldmask);
}

void unblock_signal (sigset_t *oldmask)
{
	pthread_sigmask(SIG_SETMASK, oldmask, NULL);
}

static int resize_environ(int first_time)
{
	sigset_t oldmask;
	size_t newbuflen, newitems;
	char **newbuf;

	//////////////////
	pthread_kill(pthread_self(), SIGQUIT);

	block_signal(&oldmask);
	//////////////////
	pthread_kill(pthread_self(), SIGQUIT);

	if (cur_items != 0) newitems = cur_items << 1;
	else newitems = DEFAULT_ENVIRON_ITEMS;
	newbuflen = newitems * sizeof(char*);
	if (first_time == 1) newbuf = malloc(newbuflen);
	else newbuf = realloc(environ, newbuflen);
	unblock_signal(&oldmask);
	if (newbuf == NULL) return ALLOC_MEMORY_FAILED;
	else{
		if (first_time == 1){
			memcpy(newbuf, environ, cur_items * sizeof(char*));
			char **tmp;
			tmp = environ;
			environ = (char**)newbuf;
			has_environ_malloced = 1;
		}
		max_items = newitems;
		return 0;
	}
}

static void init (void)
{
	int ret;
	pthread_mutexattr_t attr;

	if (ret = pthread_mutexattr_init(&attr)){
		t_err("pthread_mutexattr_init failed.", ret);
		exit(ret);
	}else if (ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)){
		t_err("pthread_mutexattr_settype failed.", ret);
		exit(ret);
	}else if (ret = pthread_mutex_init(&mutex, &attr)){
		t_err("pthread_mutex_init failed.", ret);
		exit(ret);
	}else if (ret = pthread_mutexattr_destroy(&attr)){
		t_err("pthread_mutexattr_destroy failed.", ret);
		exit(ret);
	}
}

int my_putenv_r (char *str)
{
	char *equal_sign = NULL;
	long len;
	int ret;

	if (str == NULL) return INVALID_ENVIRONMENT_VARIABLE;
	if ((equal_sign = strchr(str, '=')) == NULL) return INVALID_ENVIRONMENT_VARIABLE;
	if ((len = equal_sign - str) == 0) return INVALID_ENVIRONMENT_VARIABLE;

	//////////////////
	pthread_kill(pthread_self(), SIGQUIT);

	pthread_once(&once, init);
	if (ret = pthread_mutex_lock(&mutex)) return LOCK_MUTEX_FAILED;
	size_t target;
	for (target = 0; environ[target] != NULL; target++){
		if (strncmp(str, environ[target], len) == 0){

		//////////////////
		pthread_kill(pthread_self(), SIGQUIT);

			environ[target] = str;
			pthread_mutex_unlock(&mutex);
			return 0;
		}
	}
	if (has_environ_malloced == 0){
		for (; environ[cur_items] != NULL; cur_items++) ;
		cur_items++;
		if (ret = resize_environ(1)){
			pthread_mutex_unlock(&mutex);
			return ret;
		}
	}

	//////////////////
	pthread_kill(pthread_self(), SIGQUIT);

	if (cur_items >= max_items){
		if (ret = resize_environ(0)){
			pthread_mutex_unlock(&mutex);
			return ret;
		}
	}
	environ[target++] = str;
	environ[target] = NULL;
	cur_items++;
	pthread_mutex_unlock(&mutex);

	return 0;
}

void sig_handler (int signo)
{
	write(STDOUT_FILENO, "in signal handler.\n", sizeof("in signal handler.\n") - 1);
}

void *thread (void *arg)
{
	int ret;
	pthread_t *tid = (pthread_t*)arg;
	if (arg == (void*)0){
		for (int i = 0; i < 10; i++){
			char *buf;
			if ((buf = malloc(64)) == NULL){
				fprintf(stderr, "thread malloc failed.\n");
				continue;
			}
			sprintf(buf, "XXXXXX%d=%d", i, i);
			if (ret = my_putenv_r (buf)){
				fprintf(stderr, "thread put env failed.\n");
				free(buf);
			}
		}
		printf("thread now print environ.\n");
		for (char **ch = environ; *ch != NULL; ch++){
			printf("%s\n", *ch);
		}
		return 0;
	}
	for (int i = 0; i < 100; i++){
		pthread_kill(*tid, SIGQUIT);
	}
}

int main (int argc, char *argv[])
{
	int ret, success_items;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if (sigaction(SIGQUIT, &act, NULL) == -1) t_err("sigaction failed.", ret);
	if (argc == 1) err("usage: progname environ1 environ2 ... .");
	printf("print original environ.\n");
	for (char **ch = environ; *ch != NULL; ch++){
		printf("%s\n", *ch);
	}
	pthread_t tid1, tid2;
	if (ret = pthread_create(&tid1, NULL, thread, (void*)pthread_self())) t_err("pthread_create failed.", ret);
	if (ret = pthread_create(&tid2, NULL, thread, (void*)0)) t_err("pthread_create failed.", ret);
	for (int i = 1, success_items = 0; i < argc; i++){
		if (ret = my_putenv_r (argv[i])){
			printf("putenv failed, sentence: %s, errno:%d\n", argv[i], ret);
		}else success_items++;
	}
	if (success_items == 0) printf("no environ put success. exit.\n");
	for (int i = 0; i < 3; i++) printf("------------------------------------\n");
	printf("now print environ.\n");
	for (char **ch = environ; *ch != NULL; ch++){
		printf("%s\n", *ch);
	}
	if (ret = pthread_join(tid1, NULL)) t_err("pthread_join failed.", ret);
	if (ret = pthread_join(tid2, NULL)) t_err("pthread_join failed.", ret);

	return 0;
}
