#ifndef SEISLABEL_H
#define SEISLABEL_H

#include <QtGui>
#include <QLabel>
#include <QFont>
#include "seisview.h"

enum TypeLabel{Vertical,Horizonal};
class SeisLabel : public QFrame
{
    Q_OBJECT
public:
    SeisLabel();

    SeisLabel(ShowSlice *slice,DataManager *dm,TypeLabel Type);
    ~SeisLabel();
    void UpdateData();              //赋值更新数据
private:
    void paintEvent(QPaintEvent *e);
    void drawVerticalLabel(QPainter* painter);
    void drawHorizonalLabel(QPainter* painter);
private:
    ShowSlice *slice;
    DataManager *dm;
    int X_interval;
    int T_interval;
    int X_Min,Y_Min;
    int dx,dy;
    int lx,ly;
    float XScale,YScale;
    int N,M;
    int Width,Height;
    TypeLabel Type;

signals:
    
public slots:
    void getLabel(int data);
    
};

#endif // SEISLABEL_H
