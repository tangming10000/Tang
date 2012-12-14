#include "seisdraw.h"

Seis_Trace::Seis_Trace()
{
    X = Y = 0;
    Start_Time = End_Time = 0;
    N = 0;
    data = NULL;
}

Seis_Trace::Seis_Trace(int x, int y, int start_time, int end_time, int N, float *in_data)
{
    X = x;
    Y = y;
    Start_Time = start_time;
    End_Time = end_time;
    if ( N > 0 )
    {
        if( data != NULL )
        {
            delete [] data;
            data = NULL;
        }

        data = new float[N];
        for( int i = 0; i < N; i++ )
        {
            data[i] = in_data[i];
        }
    }
}

Seis_Trace::~Seis_Trace()
{
    if( data != NULL )
    {
        delete [] data;
        data = NULL;
    }
}

void Seis_Trace::SetPos(int x, int y)
{
    X = x;
    Y = y;
}

void Seis_Trace::SetTime(int start_time, int end_time)
{
    Start_Time = start_time;
    End_Time = end_time;

}

void Seis_Trace::SetData(int N, float *in_data)
{
    if ( N > 0 )
    {
        if( data != NULL )
        {
            delete [] data;
            data = NULL;
        }
        data = new float[N];
        for( int i = 0; i < N; i++ )
        {
            data[i] = in_data[i];
        }
    }

}

//ÉèÖÃÆÊÃæ
Seis_Slice::Seis_Slice()
{
    Index = 0;
}

Seis_Slice::Seis_Slice(int Idx)
{
    Index = Idx;
}

Seis_Slice::~Seis_Slice()
{

}
