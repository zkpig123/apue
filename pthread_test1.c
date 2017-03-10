#include "assist.h"

pthread_mutex_t mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct info_t{
	pthread_t tid;
	char *msg;
	struct info_t *next;
}*info_list;

void enqueue_info (struct info_t *info){
	struct info_t *t;
	info->next = info_list;
	info_list = info;
}

void clean_func (void *arg)
{
	struct info_t *info;
	info = (struct info_t*)arg;
	if (info != NULL){
		info = (struct info_t*)arg;
		printf("clean func: from %s.\n", info->msg);
		free(info);
	}else printf("clean func: info null.\n");
}

void cancel_clean_func (void *arg)
{
	printf("cancel clean func.\n");
}

void *thread (void *arg)
{
	struct info_t *info = NULL;
	pthread_t tid;
	int ret;

	printf("into thread.\n");
	if (arg == (void*)1){
		pthread_cleanup_push(cancel_clean_func, NULL);
		if (ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) fprintf(stderr, "pthread_setcancelstate failed.\n");
		else printf("set cancel state ok.\n");
		if (ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) fprintf(stderr, "pthread_setcanceltype failed.\n");
		else printf("set cancel type ok.\n");
		pause();
		pthread_cleanup_pop(0);
	}else sleep(1);
	tid = pthread_self();
	pthread_mutex_lock(&mutex);
	while (1){
		pthread_cond_wait(&cond, &mutex);
		struct info_t *prev;
		for (info = info_list, prev = NULL; info != NULL; prev = info, info = info->next){
			if (pthread_equal(tid, info->tid)){
				if (info == info_list){
					info_list = info->next;
				}
				else prev->next = info->next;
				pthread_mutex_unlock(&mutex);
				goto afterwhile;
			}
		}
	}
afterwhile: ;
	pthread_cleanup_push(clean_func, (void*)info);
	if (strcmp(info->msg, "return") == 0){
		printf("return now.\n");
		return 0;
	}
	else if (strcmp(info->msg, "pthread_exit") == 0){
		printf("pthread_exit now.\n");
		pthread_exit(0);
	}
	else if (strcmp(info->msg, "exit") == 0){
		sleep(2);
		printf("exit now.\n");
		exit(0);
	}else if (strcmp(info->msg, "cancel") == 0){
		pthread_t *cancel_id;
		cancel_id = (pthread_t*)arg;
		printf("pthread_cancel now.\n");
		int ret;
		if (ret = pthread_cancel(*cancel_id)){
			printf("pthread_cancel failed, errno:%d\n", ret);
		}
	}
	printf("thread: shouldn't got here. msg:%s\n", info->msg);
	pthread_cleanup_pop(1);
	pthread_exit((void*)1);
}

int main (void)
{
	pthread_t tids[5];
	char *msg[] = {"return", "pthread_exit", "exit", "cancel", NULL};
	int ret;
	pthread_mutexattr_t attr;
	
	if (ret = pthread_mutexattr_init(&attr)) t_err("pthread_mutexattr_init failed.", ret);
	if (ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)) t_err("pthread_mutexattr_settype failed.", ret);
	if (ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE)) t_err("pthread_mutexattr_setpshared failed.", ret);
	if (ret = pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST)) t_err("pthread_mutexattr_setrobust failed.", ret);
	if (ret = pthread_mutex_init(&mutex, &attr)) t_err("pthread_mutex_init failed.", ret);
	if (ret = pthread_mutexattr_destroy(&attr)) t_err("pthread_mutexattr_destroy failed.", ret);

	if (ret = pthread_create(&tids[0], NULL, thread, (void*)0)) fprintf(stderr, "pthread_create failed, errno:%d\n", ret);
	if (ret = pthread_create(&tids[1], NULL, thread, (void*)0)) fprintf(stderr, "pthread_create failed, errno:%d\n", ret);
	if (ret = pthread_create(&tids[2], NULL, thread, (void*)0)) fprintf(stderr, "pthread_create failed, errno:%d\n", ret);
	if (ret = pthread_create(&tids[3], NULL, thread, (void*)&tids[4])) fprintf(stderr, "pthread_create failed, errno:%d\n", ret);
	if (ret = pthread_create(&tids[4], NULL, thread, (void*)1)) fprintf(stderr, "pthread_create failed, errno:%d\n", ret);
	sleep(3);
	int i = 0;
	for (char **m = msg; *m != NULL; m++, i++){
		struct info_t *info;
		if ((info = malloc(sizeof(struct info_t))) == NULL) err("malloc failed.");
		info->tid = tids[i];
		info->msg = *m;
		info->next = NULL;
		pthread_mutex_lock(&mutex);
		enqueue_info(info);
		pthread_mutex_unlock(&mutex);
	}
	pthread_cond_broadcast(&cond);
	for (int i = 0; i < sizeof(tids); i++){
		int ret;
		if (ret = pthread_join(tids[i], NULL)) printf("pthread_join failed, errno:%d\n", ret);
	}

	return 0;
}
