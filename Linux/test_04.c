#include <stdio.h>
int Fib(int n)
{
	if ( n == 0 )
	  return 1;
	else if( n == 1 )
	  return 1;
	else 
	  return Fib(n-1)+Fib(n-2);
}

int main(void)
{
	printf("The result of Fib(%d) is %d.\n",10,Fib(10));
	return 0;
}
