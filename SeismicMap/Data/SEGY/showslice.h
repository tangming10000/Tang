#ifndef SHOWSLICE_H
#define SHOWSLICE_H

#include <QList>
#include <QtAlgorithms>
#include "showtrace.h"

class ShowTrace;

class ShowSlice
{
    //保存地震剖面数据
public:
    ShowSlice(int Idx);
    ~ShowSlice();
    void clear();
public:
    int Index;
    QList<ShowTrace*> slice;
};

#endif // SHOWSLICE_H
