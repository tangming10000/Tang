#ifndef SEISWIDGET_H
#define SEISWIDGET_H

#include <QtGui>
#include <QWidget>
#include "seisview.h"
#include "seislabel.h"

class SeisView;
class SeisLabel;

class SeisWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SeisWidget(SeisView *view = 0);

private:
    SeisView *seisview;
    SeisLabel *seisLabel;

    void InitLayout();

signals:
    
public slots:
};

#endif // SEISWIDGET_H
