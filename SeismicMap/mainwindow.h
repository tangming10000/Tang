#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Data/DataManager/datamanager.h"
#include "Interface/SeisView/seisview.h"
#include "Interface/MapView/mapview.h"
#include "Interface/GLWidget/glwidget.h"
#include "Interface/SeisView/seiswidget.h"

class DataManager;
class SeisView;
class SeisWidget;
class MapView;
class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool Connect_DataBase();
private:
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *e);

public:
    DataManager *dm;
    SeisView *seisView;
    SeisWidget *seisWidget;
    MapView *mapView;
    GLWidget *glWidget;

private:
    QMdiArea *mdiArea;

    QToolBar *window_ToolBar;   //窗口选择工具栏

    QAction *action_Show_Seismic;
    QAction *action_Show_Map;
    QAction *action_Show_OpenGL;

private:
    void Init_Layout();
    void create_ToolBar();
    void create_Actions();
public slots:
    //slots for actions
    void slot_Show_Seismic();
    void slot_Show_Map();
    void slot_Show_OpenGL();

};

#endif // MAINWINDOW_H
