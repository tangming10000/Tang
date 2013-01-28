#ifndef SEISBACK_H
#define SEISBACK_H

#include <QtGui>
#include <QGraphicsItem>
#include "seisview.h"

class SeisBack : public QGraphicsItem
{
public:
    SeisBack(ShowSlice *slice, DataManager *dm);
    ~SeisBack();
private:
    DataManager *dm;
    ShowSlice *slice;
    int Num_Trace;      //当前总道数
    int Num_HNS;        //采样点数
    float dt;
    int dx,dy;
    int Width,Height;
    int X_interval,Y_interval;

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void drawGrid(QPainter *painter);       //绘制网格
    void drawLabel(QPainter *painter);      //绘制标签刻度
};

#endif // SEISBACK_H
