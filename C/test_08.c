/*
   本程序主要详细介绍数组及数组指针的应用
*/
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int i,j;
	float **data;
	int M = 10;
	int N = 12;
	data = (float**)malloc(M*sizeof(float*));
	for( i = 0; i < M; i++ )
	  data[i] = (float*)malloc(N*sizeof(float));

	for( i = 0; i < M; i++ )
	{
		for( j = 0; j < N; j++ )
		  data[i][j] = i*N+j;
	}

	for( i = 0; i < M; i++ )
	{
		for( j = 0; j < N; j++ )
		{
			printf("%4.2f   ",*(*(data+i)+j));
		}
		printf("\n");
	}
	for( i = 0; i < M; i++ )
	  free(data[i]);
	free(data);

	return 0;
}
