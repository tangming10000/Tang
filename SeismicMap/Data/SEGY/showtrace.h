#ifndef SHOWTRACE_H
#define SHOWTRACE_H

#include "segy.h"

class ShowTrace
{
    //保存地震单道地震数据
public:
    ShowTrace(int Idx);
    ShowTrace(int Idx,FILE* fp, SEGYHEAD* segyhead,long pos);
    ~ShowTrace();
    void LoadTrace(int Idx , FILE* fp, SEGYHEAD* segyhead, long pos);
public:
    int Index;
    TRACE *trace;
    float *data;
    float X,Y;        //XY坐标
};

#endif // SHOWTRACE_H
