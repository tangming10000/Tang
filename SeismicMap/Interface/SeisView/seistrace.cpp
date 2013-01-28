#include "seistrace.h"

SeisTrace::SeisTrace(ShowTrace *Trace, SeisSetting *setseis)
{
    trace = Trace;
    seisset = setseis;
    dx = seisset->dx * seisset->XScale;
    dy = seisset->dy * seisset->YScale;
    Start_MutTime = trace->trace->muts;
    End_MutTime = trace->trace->mute;
    M = trace->trace->ns;
    Width = seisset->dx*2;
    Height= seisset->dy*M;
    dx *= seisset->gain;
    isDraw= true;
}

SeisTrace::~SeisTrace()
{
    trace = NULL;
    seisset = NULL;
}
QRectF SeisTrace::boundingRect() const
{
    return QRectF(-Width/2,-Height/2,Width,Height);
}

void SeisTrace::draw_WaveArea(QPainter *painter)//²¨ĞÎ±äÃæ»ıÍ¼Ïñ
{
    QPoint p1,p2;
    //painter->setPen(QPen(color_WaveLine,pen_width,Qt::SolidLine,Qt::RoundCap));
    painter->setPen(QPen(seisset->color_Line,seisset->width_Line,\
                         Qt::SolidLine,Qt::RoundCap));
    if (isDraw == true)
    {
        float d1,d2;
        d1 = trace->data[0]*dx;
        for( int j = 0; j < M-1; j++)
        {
            d2 = trace->data[j+1]*dx;
            p1.setX(d1);
            p1.setY(j*dy);
            p2.setX(d2);
            p2.setY((j+1)*dy);
            painter->drawLine(p1,p2);
            d1 = d2;
        }
        //Ìî³äÍ¼Ïñ
        QPolygon poly;
        //painter->setBrush(QBrush(color_Area,Qt::SolidPattern));
        painter->setBrush(QBrush(seisset->color_Area,Qt::SolidPattern));
        d1 = trace->data[0]*dx;
        for( int j = 0; j < M-1; j++)
        {
            d2 = trace->data[j+1]*dx;
            if(d1 <= 0 && d2 >0)
            {
                int y;
                poly.clear();
                y = -d1*dy/(d2-d1);
                p1.setX(0);
                p1.setY(j*dy+y);
                poly.push_back(p1);
            }
            else if( d1 > 0 && d2 > 0 )
            {
                p1.setX(d1);
                p1.setY(j*dy);
                poly.push_back(p1);
            }
            else if( d1> 0 && d2 <= 0)
            {
                int y;
                p1.setX(d1);p1.setY(j*dy);
                poly.push_back(p1);
                y = d1*dy/(d1-d2);
                p1.setX(0);
                p1.setY(j*dy+y);
                poly.push_back(p1);
                painter->drawPolygon(poly);
                poly.clear();
            }
            d1 = d2;
        }
    }
    else
    {
        painter->drawLine(0,0,0,Height);
    }
}

void SeisTrace::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    painter->translate(0,-Height/2.0);
    draw_WaveArea(painter);
}
