#ifndef SEISDRAW_H
#define SEISDRAW_H
#include <QList>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "segy.h"

typedef struct
{
    //建立三维观测系统
    int Start_T,Start_X,Start_Y;    //开始坐标值大小
    int End_T,End_X,End_Y;          //终止坐标值大小
    int Interval_T,Interval_X,Interval_Y;   //图像显示时，网格的间隔大小
    float hdt;           //采样时间间隔(ms)
}SEIS_CUBE;

class Seis_Trace
{
    //保存单道地震数据
public:
    Seis_Trace();
    Seis_Trace(int x, int y, int start_time,int end_time, int N, float* in_data);
    ~Seis_Trace();
public:
    void SetPos(int x,int y);
    void SetTime(int start_time, int end_time);
    void SetData(int N,float* in_data);
public:
    //单道地震数据
    int X,Y;//该道地震数据在观测系统中的XY坐标
    int Start_Time,End_Time;  //当前的地震数据的开始、结束时间
    int N;      //N个采样点数据
    float *data;//地震数据
};

class Seis_Slice
{
    Seis_Slice();
    Seis_Slice(int Idx);
    ~Seis_Slice();
    //保存设置地震剖面
    int Index;      //当前剖面的编号

};

#endif // SEISDRAW_H
