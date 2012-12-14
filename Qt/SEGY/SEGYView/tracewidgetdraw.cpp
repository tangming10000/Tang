#include "tracewidgetdraw.h"

TraceWidgetDraw::TraceWidgetDraw(TraceWidget *parent) :
    QWidget(parent)
{
    tracewidget = parent;
    N=5;
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    isDraw=false;
    //for(int i =0; i < N; i++)
    Max = 0.0;
}
TraceWidgetDraw::~TraceWidgetDraw()
{
    Data.clear();
    std::vector<float>().swap(Data);
}

void TraceWidgetDraw::paintEvent(QPaintEvent *e)
{
    Max = 0.0;
    if (isDraw)
    {
    for(int i = 0; i < N;i++)
    {
        float temp = Data[i];
        if (Max < abs(temp))
            Max = temp;
    }
    w = this->width();
    h = this->height();
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing,true);
    p.setPen(QPen(Qt::blue,3,Qt::SolidLine,Qt::RoundCap));
    p.translate(10,10);
    w = w-20;
    p.drawLine(0,0,w,0);
    p.translate(w/2,0);
    h = h-20;
    qreal st = h/qreal(N);
    qreal sx = w/2.0/Max;

    p.setPen(QPen(Qt::black,0.5,Qt::DotLine,Qt::RoundCap));
    for(int i =-5; i <= 5; i++)
        p.drawLine(i*(w/10),0,i*(w/10),h);
    for(int i = 100; i < N; i += 100)
        p.drawLine(-w/2.0,i*st,w/2.0,i*st);

//    for(qreal i = st; i <= h; i += st*100)
 //       p.drawLine(-w/2.0,i,w/2.0,i);

    //p.drawLine(0,0,0,h);
    //p.translate(w/2+10,10);
    p.setPen(QPen(Qt::black,1,Qt::SolidLine,Qt::RoundCap));

    QPoint point[N] ;
    for(int i = 0; i < N; i++)
    {
        point[i].setX(sx*Data[i]);
        point[i].setY(i*st);
    }
    p.drawPolyline(&point[0],N);
    }
}
void TraceWidgetDraw::resizeEvent(QResizeEvent *e)
{
    w = this->width();
    h = this->height();
    update();
}

void TraceWidgetDraw::SetData(float x[],int N)
{
    Data.erase(Data.begin(),Data.end());
    isDraw= true;
    for(int i = 0; i < N; i++)
        Data.push_back(x[i]);
    update();
}
