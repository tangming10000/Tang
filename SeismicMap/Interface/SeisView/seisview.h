#ifndef SEISVIEW_H
#define SEISVIEW_H
#include <QtGui>
#include <QGraphicsView>
#include "../../mainwindow.h"
#include "seistrace.h"
#include "seisback.h"
#include "seislabel.h"

class DataManager;
class ShowTrace;
class SeisTrace;
class SeisLabel;

class SeisView : public QGraphicsView
{
    Q_OBJECT
public:
    SeisView();
    ~SeisView();
    DataManager* dm;
    ShowSlice *slice;

    void SetData(DataManager* dm);
    void DrawSlice(ShowSlice *slice);
    void CreateTopLabel(ShowSlice* slice,DataManager* dm);
private:
    QGraphicsScene *scene;
    int dx,dy;
    int Width,Height;
signals:
    
public slots:
    
};

#endif // SEISVIEW_H
