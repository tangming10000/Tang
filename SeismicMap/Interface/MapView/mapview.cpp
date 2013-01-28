#include "mapview.h"

MapView::MapView()
{
    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    scene = new QGraphicsScene(this);
    setCacheMode(CacheBackground);
    setScene(scene);
    scene->setSceneRect(-320,-240,640,480);
    setWindowTitle(tr("Seismic Map"));
    dm = NULL;
}

MapView::~MapView()
{
    dm = NULL;
}

void MapView::SetData(DataManager *DM)
{
    dm = DM;
}

void MapView::DrawMap()
{
     Width = (dm->mapset->Max_XLine - dm->mapset->Min_XLine) + 60;
     Height= (dm->mapset->Max_YLine - dm->mapset->Min_YLine) + 60;
     scene->setSceneRect(-Width/2.0,-Height/2.0,Width,Height);
     MapItem *item = new MapItem(dm);
     scene->addItem(item);
     item->setPos(0,0);
}
