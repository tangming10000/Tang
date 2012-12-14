#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>

void *thread(void* arg);
char Message[]="Hello,dear!!";

int main()
{
	int res;
	pthread_t t01;
	void* thread_result;
	res = pthread_create(&t01,NULL,thread,(void*)Message);
	if( res != 0 )
	{
		printf("Thread creation failed!!\n");
		exit(EXIT_FAILURE);
	}

	printf("Waiting for thread to finish...\n");
	res = pthread_join(t01,&thread_result);
	if( res != 0 )
	{
		printf("Thread join failed!!\n");
		exit(EXIT_FAILURE);
	}

	printf("Thread joined,it returned %s.\n",(char*)thread_result);
	printf("Message is now %s.\n",Message);
	return 0;
}

void* thread(void* arg)
{
	printf("Thread function is running.Argument was %s.\n",(char*)arg);
	sleep(3);
	strcpy(Message,"Bye~!");
	pthread_exit("Thank you for the cpu time!!");
}
