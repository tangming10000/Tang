#ifndef TRACEWIDGET_H
#define TRACEWIDGET_H
#include "mainwindow.h"
#include "tracewidgetdraw.h"
#include <QtGui/QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextCodec>
#include <QSpinBox>
#include <QSlider>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "segy.h"
#include <QtGui/QWidget>
#include <QFile>
#include <QDataStream>
#include <vector>

class TraceWidgetDraw;
class MainWindow;
class QSpinBox;
class QSlider;
class QTableWidget;
class QTableWidgetItem;
class REEL;
class TRACE;
class TraceWidget : public QWidget
{
    Q_OBJECT

public:
    TraceWidget(MainWindow *parent);
    ~TraceWidget();
    QString name;   //SEGY file name
    REEL Reel;//SEGY REEL Header
    TRACE Trace; //SEGY Traces Header
    int fp;
    int TraceAll[71];
    MainWindow *mainWindow;

private slots:
    //void Changfp(int);
    void ShowTrace(int tfp);
public:
    void output();
    void resizeEvent(QResizeEvent *);
protected:
    QSpinBox *spinBox;
    QSlider *slider;
    QTableWidget *table;
    QTableWidget *tableData;
    TraceWidgetDraw *DWidget;
};

#endif // WIDGET_H
