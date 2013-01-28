#include "mapitem.h"

MapItem::MapItem(DataManager *dm)
{
    X_Min = dm->mapset->Min_XLine;
    X_Max = dm->mapset->Max_XLine;
    Y_Min = dm->mapset->Min_YLine;
    Y_Max = dm->mapset->Max_YLine;
    Width =  (X_Max - X_Min);
    Height = (Y_Max - Y_Min);

    Interval = 50;
}


QRectF MapItem::boundingRect() const
{
    return QRectF(-Width/2-20,-Height/2-20,Width+40,Height+40);
}
void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    painter->translate(-Width/2.0,-Height/2.0);
    painter->setPen(QPen(Qt::blue,4,Qt::SolidLine,Qt::RoundCap));
    painter->drawRect(0,0,Width,Height);
    drawMap(painter);
    drawGrid(painter);
    drawLabel(painter);
}

void MapItem::drawMap(QPainter *painter)
{
    //painter->drawRect(0,0,Width,Height);
    painter->drawLine(0,0,0,Height);
    painter->drawLine(0,Height,Width,Height);
    painter->drawLine(Width,Height,Width,0);
    painter->drawLine(Width,0,0,0);
}

void MapItem::drawGrid(QPainter *painter)
{
    painter->setPen(QPen(Qt::blue,2,Qt::DotLine,Qt::RoundCap));
    int N = Width / Interval;
    int M = Height / Interval;
    for( int i = 0; i < N; i++ )
        painter->drawLine(i*Interval,0,i*Interval,Height);
    for( int i = 0; i < M; i++)
        painter->drawLine(0,i*Interval,Width,i*Interval);
}

void MapItem::drawLabel(QPainter *painter)
{
    painter->setPen(QPen(Qt::blue,2,Qt::SolidLine,Qt::RoundCap));
    int N = Width / Interval;
    int M = Height / Interval;
    for( int i = 0; i <= N; i++ )
    {
        painter->drawText(i*Interval-10,-10,QString::number(X_Min+i*Interval));
        painter->drawText(i*Interval-10,Height+15,QString::number(X_Min+i*Interval));
    }
    for( int i = 0; i <= M; i++ )
    {
        painter->drawText(-30,    i*Interval+5,QString::number(Y_Min+i*Interval));
        painter->drawText(Width+5,i*Interval+5,QString::number(Y_Min+i*Interval));
    }

}
