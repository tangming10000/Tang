#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUM 10
static void* print_tid(void* arg)
{
	int* t = malloc(sizeof(int));
	*t = *((int*)arg);
	printf("Thread id:%d,return addr:%p.\n",*((int*)arg),t);
	return t;
}

int main()
{
	int status;
	int i,tid[THREAD_NUM],*ret[THREAD_NUM];
	pthread_t pt[THREAD_NUM];
	for(i = 0; i < THREAD_NUM;i++ )
	{
		tid[i] = i;
		status = pthread_create(&pt[i],NULL,print_tid,&tid[i]);
		if( status != 0 )
		  fprintf(stderr,"Create thread %d fail!!!\n",i);
	}

	for(i = 0; i < THREAD_NUM;i++ )
	{
		status = pthread_join(pt[i],(void**)(&ret[i]));
		if( status != 0 )
		  fprintf(stderr,"join thread %d  fail!\n",i);
	}

	for(i = 0; i < THREAD_NUM;i++ )
	{
		printf("thread return %d,return addr:%p.\n",*ret[i],ret[i]);
		free(ret[i]);
	}
	return 0;
}
