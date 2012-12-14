#include <stdio.h>
#include <stdlib.h>

int main()
{
	char filename[20];
	int i,n;
	for( i = 0; i < 100; i++ )
	{
		n = sprintf(filename,"%s%d%s","filename",i,".txt");
		printf("%s\n",filename);
	}
	return 0;
}
