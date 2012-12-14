#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

pthread_t id;
int ret;

void thread()
{
	while(1)
	{
		printf("I'm thread!!\n");
		sleep(1);
	}
}

int main()
{
	ret = pthread_create(&id,NULL,(void*)thread,NULL);
	if( ret != 0 )
	{
		printf("Create pthread error!!\n");
		exit(-1);
	}
	while(1)
	{
		printf("I'm the main thread!!\n");
		sleep(1);
	}
	return 0;
}
