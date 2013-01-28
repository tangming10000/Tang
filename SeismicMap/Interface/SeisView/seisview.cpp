#include "seisview.h"

SeisView::SeisView()
{
    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    scene = new QGraphicsScene(this);
    setCacheMode(CacheBackground);
    setScene(scene);
    scene->setSceneRect(-320,-240,640,480);
    showMaximized();
    setWindowTitle(tr("Seismic Profiles"));
    connect(horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(update()));
    connect(verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(update()));
}
SeisView::~SeisView()
{
    dm = NULL;
    slice = NULL;
}

void SeisView::SetData(DataManager *DM)
{
    dm = DM;
}

void SeisView::DrawSlice(ShowSlice *mslice)
{
    slice = mslice;
    if(slice->slice.size() > 0 )
    {
        CreateTopLabel(slice,dm);
        int N = slice->slice.size();
        int M = dm->Reel->hns;
        dx = dm->seisset->dx*dm->seisset->XScale*2;
        dy = dm->seisset->dy*dm->seisset->YScale;
        Width = N*dx+dm->seisset->lx*2;
        Height= M*dy+dm->seisset->ly*2;

        scene->setSceneRect(-Width/2.0,-Height/2.0,Width,Height);
        SeisBack *seisBack = new SeisBack(slice,dm);
        scene->addItem(seisBack);
        for( int i = 0; i < N; i++ )
        {
            //SeisTrace *trace =new SeisTrace(&(slice->Trace[i].data));
            SeisTrace *trace = new SeisTrace(slice->slice[i],dm->seisset);
            scene->addItem(trace);
            trace->setPos((i-N/2+1)*dx,0);
        }

        seisBack->setPos(0,0);
    }
}

void SeisView::CreateTopLabel(ShowSlice *slice, DataManager *dm)
{
//    int StartX = slice->Trace[0].X;
//    int StartY = slice->Trace[0].Y;

    /*
    QFrame *frame = new QFrame;
    QLabel *label1 = new QLabel(tr("Show Top Labels"));

    QPushButton *bt1 = new QPushButton(tr("Label Show"));
    bt1->setPalette(QPalette(Qt::red));
    bt1->setAutoFillBackground(true);

    QHBoxLayout *layout_frame = new QHBoxLayout;
    layout_frame->addWidget(label1);
    layout_frame->setMargin(0);
    //layout_frame->addWidget(bt1);
    frame->setLayout(layout_frame);
    */
//    SeisLabel *seislabel = new SeisLabel(slice,dm,Horizonal);
//    connect(horizontalScrollBar(),SIGNAL(valueChanged(int)),seislabel,SLOT(getLabel(int)));
//    QVBoxLayout *layout = new QVBoxLayout(this);
//    layout->setMargin(0);
//    layout->addWidget(seislabel);
//    layout->addStretch();
//    setLayout(layout);
}
