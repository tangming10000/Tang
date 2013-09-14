#include "mainwidget.h"
#include "reader.h"
#include "newspaper.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{

    NewsPaper news("NewsPaper A");
    Reader reader;
    QObject::connect(&news,&NewsPaper::NEWS,&reader,&Reader::Reciver);
    news.send();
}

MainWidget::~MainWidget()
{

}
