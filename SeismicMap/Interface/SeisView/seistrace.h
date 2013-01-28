#ifndef SEISTRACE_H
#define SEISTRACE_H

#include <QtGui>
#include <QGraphicsItem>
#include "seisview.h"

class ShowTrace;

class SeisTrace : public QGraphicsItem
{
public:
    SeisTrace(ShowTrace *Trace, SeisSetting *setseis);
    ~SeisTrace();
private:
    int M;              //该道的地震数据个数
    int Start_MutTime;  //开始切除时间
    int End_MutTime;    //终止切除时间
    int Width,Height;   //单道的宽度和高度
    float dx,dy;
    bool isDraw;
    ShowTrace* trace;
    SeisSetting *seisset;

    QList<float> *data;

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void draw_WaveArea(QPainter *painter);
    
};

#endif // SEISTRACE_H
