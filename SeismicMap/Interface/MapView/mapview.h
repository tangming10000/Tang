#ifndef MAPVIEW_H
#define MAPVIEW_H
#include <QtGui>
#include <QGraphicsView>
#include "mapitem.h"
#include "../../Data/DataManager/datamanager.h"

class MapItem;

class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    MapView();
    ~MapView();

    void SetData(DataManager* DM);
    void DrawMap();

public:
    DataManager* dm;
private:
    QGraphicsScene *scene;
    int dx,dy;
    int Width,Height;
signals:
    
public slots:
    
};

#endif // MAPVIEW_H
