#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

void* print_thread(void* ptr)
{
	while(1)
	{
		printf("I'm pthread!!\n");
		sleep(1);
	}
}

int main()
{
	pthread_t pid;
	pthread_create(&pid,NULL,print_thread,NULL);
	while(1)
	{
		printf("I'm the main thread!!\n");
		sleep(1);
	}
	return 0;
}
