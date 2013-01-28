#include "showtrace.h"

ShowTrace::ShowTrace(int Idx)
{
    Index = Idx;
    trace = NULL;
    data = NULL;
}

ShowTrace::ShowTrace(int Idx,FILE *fp, SEGYHEAD *segyhead, long pos)
{
    LoadTrace(Idx,fp,segyhead,pos);
}

ShowTrace::~ShowTrace()
{
    //printf("release the trace data!!\n");
    delete trace;
    delete [] data;
}

void ShowTrace::LoadTrace(int Idx,FILE *fp, SEGYHEAD *segyhead, long pos)
{
    Index = Idx;
    trace = new TRACE;
    data = new float[segyhead->hns];
    fseek(fp,pos,SEEK_SET);
    ReadTRACE(fp,trace);
    ReadData(fp,data,segyhead);
    X = trace->fldr;
    Y = trace->cdp;
//    X = trace->gx;
//    Y = trace->gy;
//    if(trace->scalco> 0)
//    {
//        X *= (float)trace->scalco;
//        Y *= (float)trace->scalco;
//    }
//    else
//    {
//        X /= float(-trace->scalco);
//        Y /= float(-trace->scalco);
//    }
//    printf("%f\t%f\n",X,Y);

    //AGC Data to (-1,1)
    float Max = 0;
    for( int i = 0; i < segyhead->hns; i++)
        if( abs(data[i]) > Max )
            Max = abs(data[i]);
    for( int i = 0; i < segyhead->hns; i++)
        data[i] /= Max;
}
