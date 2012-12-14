#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "segy.h"
#include <QtGui>
#include "tracewidget.h"
#include "glwidget.h"
#include <QDebug>
#include <QtGui/QMainWindow>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QTableView>
#include <QMenu>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QMenuBar>
#include <QEvent>
#include <QtGui/QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTableWidgetSelectionRange>
#include <QProgressDialog>
#include <QFile>
#include <QFont>
#include <QAction>
#include <QActionEvent>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include "ctrldb.h"

class QAction;
class QLabel;
class QMenu;
class QTableView;
class TraceWidget;
class REEL;
class TRACE;
class GLWidget;
class CtrlDB;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    //文件菜单
    void openFile();
    void importFile();
    void sortFile();

    //画图菜单
    void drawWave();    //波形
    void drawGray();    //灰度
    void drawColor();   //色标

    //加载地震信息
    void loadEBDIC();
    void loadReel();
    void loadTrace();
    void loadData();

    //openGL放大缩小
    void bigger();
    void little();
    void moveleft();
    void moveright();
    void moveup();
    void movedown();
    void nextSlice();
    void preSlice();
    void drawGrid();

    //操作鼠标
    void moveMouse();
    void rotateMouse();
    void setXSliceNum(int num);     /*主测线剖面*/
    void setYSliceNum(int num);     /*纵测线剖面*/
    void setTimeSliceType(int num); /*时间剖面  */

    /*旋转GLWidget图像*/
    void RotateX(int);
    void RotateY(int);
    void RotateZ(int);
    //*其他函数*//
    void XSliceInterval_Changed(int interval);

    //*Get Data from the database*//


private:
    void createActions();
    void createMenus();
    void createContextMenus();
    //void createStatusBars();
    void createToolBars();


protected:
    TraceWidget* tracewidget;
    QMainWindow * mainWindow;
    QTableWidget* table;
    GLWidget *wgl;
    CtrlDB* CdbTrace;

    QSpinBox *spin_result,*spin_interval;  /*显示当前的剖面位置或当前的时间*/
    QSlider *Xslider,*Yslider,*Zslider;
    QSpinBox *Xspin,*Yspin,*Zspin;
    QTextEdit *txtEdit;
    QMdiArea *mdiArea;
    QSqlQuery query;


    //QToolBar *drawTool;
    //QToolBar *loadTool;

    QMenu* fileMenu;
    QMenu* loadMenu;
    QMenu* drawMenu;
    QMenu* mouseFunc;

    QAction* openAction;
    QAction* importAction;
    QAction* sortAction;
    QAction* exitAction;

    QAction* ebdicAction;
    QAction* reelAction;
    QAction* traceAction;
    QAction* dataAction;

    QAction* waveAction;
    QAction* grayAction;
    QAction* colorAction;

    //操作openGL图像
    QAction* bigAction;
    QAction* littleAction;
    QAction* moveleftAction;
    QAction* moverightAction;
    QAction* moveupAction;
    QAction* movedownAction;
    QAction* nextSliceAction; /*下一条剖面*/
    QAction* preSliceAction; /*上一条剖面*/
    QAction* drawGridAction;

    //操作鼠标
    QAction* moveMouseAction;
    QAction* rotateMouseAction;
    QAction* setYSliceAction;
    //QAction* sliceTypeAction;   /*选择剖面类型XLine,CrossLine,时间剖面*/

    //水平剖面
    QAction* HerizionSlice;
    QAction* setHerizionSlice;

public:
    QString filename;
    QString CubeType;
    int SliceType;/*剖面类型*/
    REEL Reel;
    TRACE Trace;
    int XSliceNum;      /*X剖面所在的位置*/
    int XSliceInterval; /*X剖面的移动间隔*/
    int *XLine,*YLine;  /*InLine,CrossLine号保存的数组*/
    int Max_XLine,Min_XLine,Max_YLine,Min_YLine;/*InLine,CrossLine号保存的极值*/
};

#endif // MAINWINDOW_H
