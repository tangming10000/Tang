#include <stdio.h>

int GCD(int a, int b)
{
	if( a < b)
	{
		int t = a;
		a = b;
		b = t;
	}
	
	int t = a%b;
	if( t == 0 )
	{
		return b;
	}
	else
	{
		return GCD(b,t);
	}
}

int main(void)
{
	int x = 32;
	int y = 7;
	printf("The GCD of %d and %d id %d.\n",x,y,GCD(x,y));
	return 0;
}
