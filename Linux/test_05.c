#include <stdio.h>
#define N 5
int a[N] = {10,5,2,4,7};
void insertion_sort()
{
	int i,j,key;
	for( i = 1; i < N; i++ )
	{
		printf("%d,%d,%d,%d,%d\n",\
				a[0],a[1],a[2],a[3],a[4]);
		key = a[i];
		j = i - 1;
		while( j >= 0 && a[j] > key)
		{
			a[j+1] = a[j];
			--j;
		}
		a[j+1] = key;
	}
	printf("%d,%d,%d,%d,%d\n",\
		a[0],a[1],a[2],a[3],a[4]);
}
int main()
{
	insertion_sort();
	return 0;
}
