#ifndef TRACEWIDGETDRAW_H
#define TRACEWIDGETDRAW_H
#include "tracewidget.h"
#include <QWidget>
#include <QPainter>
#include <cmath>
class TraceWidget;
class TraceWidgetDraw : public QWidget
{
    Q_OBJECT
public:
    TraceWidgetDraw( TraceWidget *parent);
    ~TraceWidgetDraw();
    void paintEvent(QPaintEvent *e);
    void SetData(float x[],int N);
    void resizeEvent(QResizeEvent *e);

public:
    std::vector<float> Data;
    int N;
    qreal w,h;
    bool isDraw;
private:
    TraceWidget* tracewidget;
    qreal Max;

public slots:

};

#endif // TRACEWIDGETDRAW_H
