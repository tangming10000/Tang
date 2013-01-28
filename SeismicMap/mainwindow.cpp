#include "mainwindow.h"
#include "Data/DataBase/database.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    if(Connect_DataBase() )
    {
        dm = new DataManager();
    }
    Init_Layout();
    create_Actions();
    create_ToolBar();

}

MainWindow::~MainWindow()
{
    delete dm;
    delete seisView;
    delete seisWidget;
    delete mapView;
    delete glWidget;

    disConnection_Sqlite();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    seisView->close();
    mapView->close();

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch( e->key() )
    {
    case Qt::Key_Left:
        if( e->modifiers() & Qt::ControlModifier )
            glWidget->xSpeed = -10;
        else
            glWidget->zRot -= 18;
        break;
    case Qt::Key_Right:
        if( e->modifiers() & Qt::ControlModifier )
            glWidget->xSpeed = +10;
        else
            glWidget->zRot += 18;
        break;
    case Qt::Key_Up:
        if( e->modifiers() & Qt::ControlModifier )
            glWidget->ySpeed = -10;
        else
            glWidget->yRot -= 18;
        break;
    case Qt::Key_Down:
        if( e->modifiers() & Qt::ControlModifier )
            glWidget->ySpeed = +10;
        else
            glWidget->yRot += 18;
        break;
    case Qt::Key_PageUp:
        glWidget->zoom -= glWidget->zoom * 0.1f;
        break;
    case Qt::Key_PageDown:
        glWidget->zoom += glWidget->zoom * 0.1f;
        break;
    case Qt::Key_Plus:
        glWidget->scale += 0.1f;
        break;
    case Qt::Key_Minus:
        glWidget->scale -= 0.1f;
        break;
    case Qt::Key_S:
        glWidget->killTimer(glWidget->t);
        break;
    case Qt::Key_R:
        glWidget->t = startTimer(50);
        break;
    case Qt::Key_Home: //回到初始状态
        if( e->modifiers() & Qt::ControlModifier )
            glWidget->Go_init();
        break;
    case Qt::Key_X: //观察X面
        if( e->modifiers() & Qt::ControlModifier )
        {
            glWidget->Go_init();
            glWidget->zRot = 90;
        }
        break;
    case Qt::Key_Y:
        if( e->modifiers() & Qt::ControlModifier )
        {
            glWidget->Go_init();
            glWidget->zRot =180;
        }
        break;
    case Qt::Key_Z:
        if( e->modifiers() & Qt::ControlModifier )
        {
            glWidget->Go_init();
            glWidget->xRot = 90;
        }
        break;
    case Qt::Key_Escape:
        glWidget->close();
        break;
    }
    glWidget->updateGL();
}

bool MainWindow::Connect_DataBase()
{
    bool flags = false;

#ifdef WIN32
    QString filename = QObject::tr("E:/SEGY_Data/dynp_dq.sgy");
    QString dbname = QObject::tr("C:/Users/Tang/test.db");
#else
    QString filename = QObject::tr("/media/Backup/SEGY_Data/dynp_dq.sgy");
    QString dbname = QObject::tr("/home/tommy/test.db");
#endif

    //判断数据库文件是否存在
    QFile file(dbname);
    if(!file.exists())
        flags = true;
    if(file.size()< 5000)
        flags = true;
    file.close();

    if(!createConnection_Sqlite(dbname))
    {
        qDebug()<<"open database error!!\n";
        return false;
    }
    if( flags )
    {
        if(!db_loadSEGYHead(filename))
        {
            qDebug()<<"Load SEGY Head data to database error!!";
            return false;
        }
    }
    return true;
}

void MainWindow::Init_Layout()
{
    seisView = new SeisView();
    seisView->SetData(dm);
    if( dm->Total_Slices.size() > 0 )
    {
        seisView->DrawSlice(dm->Total_Slices.at(0));
    }

    seisWidget = new SeisWidget(seisView);

    mapView = new MapView();
    mapView->SetData(dm);
    mapView->DrawMap();


    glWidget = new GLWidget();
    glWidget->SetData(dm);

    mdiArea = new QMdiArea(this);
    mdiArea->addSubWindow(seisWidget);
    mdiArea->addSubWindow(mapView);
    mdiArea->addSubWindow(glWidget);
    this->setCentralWidget(mdiArea);

    glWidget->showMinimized();
    seisView->showMinimized();
    mapView->showMaximized();
    glWidget->hide();
}

void MainWindow::create_Actions()
{
    action_Show_Seismic = new QAction(tr("Show Seismic Profiles"),this);
    action_Show_Seismic->setIcon(QIcon(":/icons/Windows/Seismic_Profile.png"));
    action_Show_Seismic->setShortcuts(QKeySequence::Save);
    action_Show_Seismic->setToolTip(tr("Seismic Profiles(Ctrl+S)"));
    action_Show_Seismic->setEnabled(true);
    connect(action_Show_Seismic,SIGNAL(triggered()),this,SLOT(slot_Show_Seismic()));

    QList<QKeySequence> key;
    key.push_back(QKeySequence(Qt::CTRL+Qt::Key_M));
    action_Show_Map = new QAction(tr("Show Seismic Map"),this);
    action_Show_Map->setIcon(QIcon(":/icons/Windows/Seismic_Map.png"));
    action_Show_Map->setShortcuts(key);
    action_Show_Map->setToolTip(tr("Seismic Map(Ctrl+M)"));
    action_Show_Map->setEnabled(true);
    connect(action_Show_Map,SIGNAL(triggered()),this,SLOT(slot_Show_Map()));
    key.clear();

    key.push_back(QKeySequence(Qt::CTRL+Qt::Key_O));
    action_Show_OpenGL = new QAction(tr("Show OpenGL Profiles"),this);
    action_Show_OpenGL->setIcon(QIcon(":/icons/Windows/Seismic_OpenGL.png"));
    action_Show_OpenGL->setShortcuts(key);
    action_Show_OpenGL->setToolTip(tr("Seismic OpenGL Profiles(Ctrl+O)"));
    action_Show_OpenGL->setEnabled(true);
    connect(action_Show_OpenGL,SIGNAL(triggered()),this,SLOT(slot_Show_OpenGL()));

}

void MainWindow::create_ToolBar()
{
    window_ToolBar = new QToolBar(tr("Window Choose ToolBar"),this);

    window_ToolBar->addAction(action_Show_Seismic);
    window_ToolBar->addAction(action_Show_Map);
    window_ToolBar->addAction(action_Show_OpenGL);
    window_ToolBar->setFixedWidth(26);
    this->addToolBar(Qt::LeftToolBarArea,window_ToolBar);
    window_ToolBar->setOrientation(Qt::Vertical);
    window_ToolBar->show();
}


void MainWindow::slot_Show_Seismic()
{
    mapView->showMinimized();
    //glWidget->hide();
//    glWidget->showMinimized();
    seisWidget->showMaximized();
    mdiArea->update();
}

void MainWindow::slot_Show_Map()
{
    seisWidget->showMinimized();
 //   glWidget->showMinimized();

    mapView->showMaximized();
    mdiArea->update();
}

void MainWindow::slot_Show_OpenGL()
{
    seisWidget->showMinimized();
    mapView->showMinimized();
    glWidget->showMaximized();
    glWidget->show();
    mdiArea->update();
}


