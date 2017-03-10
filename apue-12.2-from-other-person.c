#include "assist.h"

extern char **environ; 
pthread_mutex_t mutex;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void thread_init(void)
{
	    pthread_mutexattr_t attr;
	        pthread_mutexattr_init(&attr);
		    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		        pthread_mutex_init(&mutex, &attr);
			    pthread_mutexattr_destroy(&attr);
}
int putenv_r(char *string)
{
	    int i, len;
	        char *ptr_key = NULL; 
		    ptr_key = strchr(string, '='); // 指向字符串中首次出现'='的地址
		        if (NULL == ptr_key)
				    {
					            printf("Param illegal Usage: name=value\n");
						            exit(0);
							        }
			    len = ptr_key - string; // 地址的差即name的长度
			        pthread_once(&init_done, thread_init);
				    pthread_mutex_lock(&mutex);
				        for (i = 0; NULL != environ[i]; i++)
						    {
							            if (0 == strncmp(string, environ[i], len))
									            {
											                environ[i] = string; // 若已存在，覆盖
													            pthread_mutex_unlock(&mutex);
														                return 0;
																        }
								        }
					    environ[i] = string;
					        pthread_mutex_unlock(&mutex);
						    return 0;
}
int main(int argc, char const *argv[])
{
	    const char *STRING = "EDITOR=vim";
	        const char *KEY = "EDITOR";
		    char *ptr = NULL;
		        char *value = NULL;
			    ptr = malloc(sizeof(char) * strlen(STRING) + 1);
			        if (NULL == ptr)
					    {
						            printf("malloc failed!\n");
							            exit(0);
								        }
				    strcpy(ptr, STRING);
				        putenv_r(ptr);
					    value = getenv(KEY);
					        if (NULL != value)
							    {
								            printf("Set %s to %s\n", value, KEY);
									        }
						    return 0;
}
