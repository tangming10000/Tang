#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 4

void* thread( void* threadid)
{
	long tid;
	tid = (long)threadid;
	printf("Hello,world! I's me, thread #%ld!\n",tid);
	pthread_exit(NULL);
}

int main(int argc, char* argv[] )
{
	pthread_t threads[NUM_THREAD];
	int rc;
	long t;
	for( t = 0; t < NUM_THREAD; t++ )
	{
		printf("In main: creating thread %ld.\n",t);
		rc = pthread_create(&threads[t],NULL,thread,(void*)t);
		if( rc != 0 )
		{
			printf("ERROR,return code from pthread_create() is %ld.\n",t);
			exit(EXIT_FAILURE);
		}
	}
	pthread_exit(NULL);
	return 0;
}
