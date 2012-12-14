#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

#define  CPU_NUM 4

double count()
{
	int N = 10000000;
	int i = 0;
	double result = 0;
	for( i = 0; i < N; i++ )
	{
		result += sin(i*0.001)*sin(i*0.001) - cos(i*0.001)*cos(i*0.001);
	}
	return result;
}

void* thread(void* arg)
{
	long t = (long)arg;
	long result = 0;
	printf("This is thread %ld.\n",t);
	result =(float) count();
	pthread_exit((void*)result);
	//pthread_exit((void*)t);
}

int main()
{
	long i = 0;
	int rec = 0;
	clock_t start,end;
	float result = 0;
	void* status;

	printf("Start the program in single thread!!\n");
	start = clock();
	for( i = 0; i < CPU_NUM; i++ )
		result +=  count();
	end = clock();
	printf("The total time in single thread is %ld,and result is %lf.\n",(end-start)/CLOCKS_PER_SEC,result);

	/*======================================*/
	printf("Start the program in multiple thread!\n");
	result = 0;
	start =clock();
	pthread_t pid[CPU_NUM];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	for( i = 0; i < CPU_NUM; i++ )
	{
		rec = pthread_create(&pid[i],NULL,thread,(void*)i);
		if(rec != 0 )
		{
			printf("ERROR,return code from pthread_create() is %ld.\n",i);
			exit(EXIT_FAILURE);
		}
	}
	pthread_attr_destroy(&attr);
	for( i = 0; i < CPU_NUM; i++ )
	{
		rec = pthread_join(pid[i],&status);
		if(rec)
		{
			printf("ERROR,return code from pthread_join() is %ld.\n",i);
			exit(EXIT_FAILURE);
		}
		//result += (float)status;
		result += (long)status;
	}
	end = clock();
	printf("The total time in multiple thread is %ld,and the result is %lf.\n",(end-start)/CLOCKS_PER_SEC,result);
	pthread_exit(NULL);
	return 0;
}
