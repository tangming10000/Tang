#include "seislabel.h"

SeisLabel::SeisLabel(ShowSlice *Slice, DataManager *DM, TypeLabel TYPE)
{
    slice = Slice;
    dm = DM;
    Type = TYPE;
    setMinimumHeight(50);
    UpdateData();
    X_Min = Y_Min = 0;
}

SeisLabel::SeisLabel()
{

}

SeisLabel::~SeisLabel()
{
    slice = NULL;
    dm = NULL;
}

void SeisLabel::UpdateData()
{
    Width = this->width();
    Height = this->height();

    dx = dm->seisset->dx*2;
    dy = dm->seisset->dy;
    lx = dm->seisset->lx;
    ly = dm->seisset->ly;
    X_interval = dm->seisset->grid_XInterval;
    T_interval = dm->seisset->grid_YInterval;
    XScale = dm->seisset->XScale;
    YScale = dm->seisset->YScale;

    N = int(float(Width) /(float(dx)*X_interval*XScale));
    M = int(float(Height)/(float(dy)*T_interval*YScale));

}

void SeisLabel::paintEvent(QPaintEvent *e)
{
    QPainter *painter = new QPainter(this);
    //painter->drawText(tr("Hello,Dear!"));
    //painter->drawLine(10,10,100,10);
    if(Type == Vertical )
        drawVerticalLabel(painter);
    else if( Type == Horizonal)
        drawHorizonalLabel(painter);
    delete painter;
}

void SeisLabel::drawVerticalLabel(QPainter *painter)
{
//    int offset = X_Min/X_interval/dx;//在当前显示地震记录中的位置
//    int mpos = (offset+1)*X_interval*dx - X_Min; //第一个绘图点据左边界的距离

//    N = (width() - mpos)/dx/X_interval;
//    QPoint P(0,0);
//    for( int i = 0; i <= N; i++ )
//    {
//        P.setX(mpos + N*X_interval*dx -15);
//        P.setY(5);
//        painter->drawText(P,QString::number(slice->Trace[offset+i*X_interval].X));
//        P.setY(15);
//        painter->drawText(P,QString::number(slice->Trace[offset+i*X_interval].Y));
//    }
}

void SeisLabel::drawHorizonalLabel(QPainter *painter)
{
    int Interval = X_interval/2;
    int offset = X_Min/dx;//在当前显示地震记录中的位置
    for(int i = 0;  i <= Interval; i++ )
    {
        int temp = slice->slice[offset+i]->Y;

        if ( temp % Interval == 0)
        {
            offset = offset+i;
            break;
        }
    }
    int mpos = (offset+1)*dx - X_Min; //第一个绘图点据左边界的距离
    painter->setFont(QFont(tr("msyh"),14));
    painter->setPen(QPen(Qt::blue,2,Qt::SolidLine,Qt::RoundCap));


    N = (width() - mpos)/dx/Interval;
    painter->drawLine(mpos+lx+dx/2,40,mpos+N*Interval*dx+lx+dx/2,40);
    //qDebug()<<N;
    QPoint P(0,0);
    for( int i = 0; i <= N; i++ )
    {
        P.setX(mpos + i*Interval*dx + lx-15);
        P.setY(15);
        painter->drawText(P,QString::number(slice->slice[offset+i*Interval]->X));
        P.setY(35);
        painter->drawText(P,QString::number(slice->slice[offset+i*Interval]->Y));
        painter->drawLine(mpos+i*Interval*dx+lx+dx/2,40,mpos+i*Interval*dx+lx+dx/2,50);
    }
}

void SeisLabel::getLabel(int data)
{
    int ww = (slice->slice.size()*dx*XScale)/2+lx;
    int hh = (dm->Reel->hns*dy*YScale )/2+ly;
    X_Min = data+ww;
    Y_Min = data+ww;
    //qDebug()<<ww<<hh<<X_Min<<Y_Min;
    update();
}


