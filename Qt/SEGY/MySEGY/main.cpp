
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include "segy.h"

int main()
{
    char* filename = "/media/Backup/SEGY_Data/segy/2.sgy";
    FILE* file = NULL;
    if( (file = fopen(filename,"rb")) == NULL )
    {
        printf("open file \"%s\" Error!\n",filename);
        exit(-1);
    }
    fseek(file,0,SEEK_SET);

    unsigned char *ebdic = NULL; //卷头EBDIC码
    REEL* Reel = NULL;  //卷头
    TRACE* Trace = NULL;//道头
    SEGYHEAD * sh = NULL;

    if( (ebdic = (unsigned char*)malloc(3200*sizeof(char))) == NULL )
    {
        printf("Get 3200 byte memory for ebdic error!\n");
        exit(-1);
    }

    if( (Reel = (REEL*)malloc(sizeof(REEL))) == NULL )
    {
        printf("Get REEL memory for REEL Header!\n");
        exit(-1);
    }

    if( (Trace = (TRACE*)malloc(sizeof(TRACE))) == NULL )
    {
        printf("Get the memory for Trace Header Error!\n");
        exit(-1);
    }
    if ( ( sh = (SEGYHEAD*)malloc(sizeof(SEGYHEAD))) == NULL )
    {
        printf("Get the memory for SEGYHeader Error!\n");
        exit(-1);
    }

//    ReadEBDIC(file,ebdic);
//    for( int i = 0; i < 3200; i++ )
//        putchar(ebdic[i]);

    ReadREEL(file,Reel);
    ReadSEGYHead(file,Reel,sh);
    printf("the total trace number is %d\t%d.\n",sh->TotalTraces,sizeof(long));

    fseek(file,3600L,SEEK_SET);
    ReadTRACE(file,Trace);
    printf("The trace number is %d.\n",Trace->dt);

    float *indata,*outdata;
    indata = (float*)malloc(sizeof(float)*1000);
    outdata= (float*)malloc(sizeof(float)*1000);
    fseek(file,3840,SEEK_SET);
    fread((void*)indata,sizeof(float),1000,file);
//    ibm2float(indata,outdata,1000);
//    for( int i = 0; i < 100; i++ )
//    {
//        for( int j = 0; j < 10; j++ )
//        {
//            printf("%.3f   ",outdata[i*10+j]);
//        }
//        printf("\n");
//    }

    fclose(file);
    free(indata);
    free(outdata);
    free(ebdic);
    free(Reel);
    free(Trace);
    return 0;

}
