#include <stdio.h>
#include <stdlib.h>

void* alloc1(int n1, int size)
{
		void *p;
		
			if ((p=malloc(n1*size))==NULL)
					return NULL;
				return p;
}
int *alloc1int(int n1)
{
		return (int*)alloc1(n1,sizeof(int));
}
float *alloc1float(int n1)
{
		return (float*)alloc1(n1,sizeof(float));
}
////////////////////////////////////\u4e8c\u7ef4
void **alloc2 (int n1, int n2, int size)
{
		int i2;
			void **p;
			
				if ((p=(void**)malloc(n2*sizeof(void*)))==NULL) 
						return NULL;
					if ((p[0]=(void*)malloc(n2*n1*size))==NULL)
						{
									free(p);
											return NULL;
												}
						for (i2=0; i2<n2; i2++)
								p[i2] = (char*)p[0]+size*n1*i2;
							return p;
}
int **alloc2int(int n1, int n2)
{
		return (int**)alloc2(n1,n2,sizeof(int));
}
float **alloc2float(int n1, int n2)
{
		return (float**)alloc2(n1,n2,sizeof(float));
}
int main()
{
	int i,j,k;
	int nx = 4;
	int nz = 3;
	float **data;
	data = alloc2float(nz,nx);
	for( i = 0; i < nx; i++ )
	{
		for( j = 0; j < nz; j++ )
		{
			data[i][j]=j;
			printf("%f  ",data[i][j]);
		}
		printf("\n");
	}	  
	FILE* file_write = fopen("data_write.dat","wb");

	fwrite(data[0],sizeof(float),nx*nz,file_write);
	fclose(file_write);

	printf("=====================================\n");
	float temp = 0;
	
	FILE* file_read = fopen("data_write.dat","rb");
	fread( data[0],sizeof(float),nz*nx,file_read );
	for( i = 0; i < nx; i++ )
	{
		for( j = 0; j < nz; j++ )
		{
			printf("%f  ",data[i][j]);
		}
		printf("\n");
	}
	fclose(file_read);
	
	return 0;
	

}
