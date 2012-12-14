#include <stdio.h>
#include <pthread.h>
#define THREAD_NUM 10
static void* print_tid(void* arg)
{
	printf("The thread id is:%d.\n",*((int*)arg));
	return arg;
}
int main()
{
	int status;
	int i,tid[THREAD_NUM];
	pthread_t pt[THREAD_NUM];
	for( i = 0; i < THREAD_NUM; i++)
	{
		tid[i] = i;
		status = pthread_create(&pt[i],NULL,print_tid,&tid[i]);
		if( status != 0 )
		  fprintf(stderr,"create thread %d fail.\n",i);
	}
	return 0;
}
