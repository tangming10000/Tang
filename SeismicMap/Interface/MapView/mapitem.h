#ifndef MAPITEM_H
#define MAPITEM_H

#include <QtGui>
#include <QGraphicsItem>
#include "mapview.h"

class DataManager;
class MapItem : public QGraphicsItem
{
public:
    MapItem(DataManager *DM);
private:
    int Width,Height;   //单道的宽度和高度
    int X_Max,X_Min,Y_Max,Y_Min;
    int Interval;       //网格间隔
private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void drawMap(QPainter *painter);        //绘制底图边界
    void drawGrid(QPainter *painter);       //绘制底图网格
    void drawLabel(QPainter *painter);      //绘制坐标
};

#endif // MAPITEM_H
