#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t ntid;
void print(const char* s)
{
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n",s,(unsigned int)pid,
			(unsigned int)tid,(unsigned int)tid);
}

void* thr_fn(void *arg)
{
	print(arg);
	return NULL;
}

int main()
{
	int err;
	err = pthread_create(&ntid,NULL,thr_fn,"new thread");
	if( err != 0 )
	{
		printf(stderr, "Can't create thread:%s.\n",strerror(err));
		exit(1);
	}
	printf("Main thread!\n");
	return 0;
}
