#include "seiswidget.h"

SeisWidget::SeisWidget(SeisView *view)
{
    seisview = view;

    InitLayout();


}

void SeisWidget::InitLayout()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    //layout->setSpacing(80);
    QVBoxLayout *layout_H = new QVBoxLayout();
    seisLabel = new SeisLabel(seisview->slice,seisview->dm,Horizonal);
    layout_H->addWidget(seisLabel);
    layout_H->addWidget(seisview);

    layout->addLayout(layout_H);
    setLayout(layout);

    connect(seisview->horizontalScrollBar(),SIGNAL(valueChanged(int)),seisLabel,SLOT(getLabel(int)));
    //connect(seisview->horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(setTopLabel(int)));
    //connect(seisview->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(setSideLabel(int)));
}



