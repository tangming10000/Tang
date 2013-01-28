#include "seisback.h"

SeisBack::SeisBack(ShowSlice *Slice,DataManager *DM)
{
    slice = Slice;
    dm =DM;
    dx = dm->seisset->dx*dm->seisset->XScale*2;
    dy = dm->seisset->dy*dm->seisset->YScale;
    Width = dx*Num_Trace;
    Height= dy*Num_HNS;
}

SeisBack::~SeisBack()
{
    dm = NULL;
    slice = NULL;
}

QRectF SeisBack::boundingRect() const
{
    return QRectF(-Width/2-40,-Height/2-40,Width+80,Height+80);
}
void SeisBack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    Num_Trace = slice->slice.size();
    Num_HNS = dm->Reel->hns;
    dt = dm->Reel->hdt/1000.0f;
    dx = dm->seisset->dx*dm->seisset->XScale*2;
    dy = dm->seisset->dy*dm->seisset->YScale;
    Width = dx*Num_Trace;
    Height= dy*Num_HNS;
    X_interval = dm->seisset->grid_XInterval;
    Y_interval = dm->seisset->grid_YInterval;


    painter->translate(-Width/2.0,-Height/2.0);
    painter->setPen(QPen(Qt::blue,4,Qt::SolidLine,Qt::RoundCap));

    painter->drawLine(0,       -dy,      0,           Height+dy);
    painter->drawLine(0,       Height+dy,Width+1.5*dx,Height+dy);
    painter->drawLine(Width+1.5*dx,Height+dy,Width+1.5*dx,-dy);
    painter->drawLine(Width+1.5*dx,-dy,      0,           -dy);
    //painter->drawRect(0,0,Width,Height);
    drawGrid(painter);
    drawLabel(painter);
}
void SeisBack::drawGrid(QPainter *painter)
{
    painter->setPen(QPen(dm->seisset->color_grid,1,Qt::SolidLine,Qt::RoundCap));
    int N = Width / X_interval / dx;
    int M = Height/ Y_interval;
    for( int i = 0; i <= N; i++ )
        painter->drawLine(i*X_interval*dx+1.5*dx,0,i*X_interval*dx+1.5*dx,Height);
    for( int i = 0; i <= M; i++ )
        painter->drawLine(1.5*dx,i*Y_interval,Width+0.5*dx,i*Y_interval);
}
void SeisBack::drawLabel(QPainter *painter)
{
    painter->setPen(QPen(dm->seisset->color_grid,1,Qt::SolidLine,Qt::RoundCap));
    int N = Width / X_interval/dx;
    int NN = Width % (X_interval*dx);
    int M = Height/ Y_interval;
    int MM = Height%Y_interval;
    for( int i = 0; i <= N; i++ )
    {
        painter->drawText(i*X_interval*dx-10,-10,QString::number(i*X_interval));
        painter->drawText(i*X_interval*dx-10,Height+15,QString::number(i*X_interval));
    }
    if(NN >= (float)X_interval/3.0)
    {
        painter->drawText(Width-10,-10,QString::number(Width/dx));
        painter->drawText(Width-10,Height+15,QString::number(Width/dx));
    }
    for( int i = 0; i <= M; i++ )
    {
        painter->drawText(-50,     i*Y_interval+5,QObject::tr("%1ms").arg(QString::number(i*Y_interval*dt/dy)));
        painter->drawText(Width+20,i*Y_interval+5,QObject::tr("%1ms").arg(QString::number(i*Y_interval*dt/dy)));
    }
    if(MM >= (float)Y_interval/3.0)
    {
        painter->drawText(-50,     Height+5,QObject::tr("%1ms").arg(QString::number(Height*dt/dy)));
        painter->drawText(Width+20,Height+5,QObject::tr("%1ms").arg(QString::number(Height*dt/dy)));
    }
}
