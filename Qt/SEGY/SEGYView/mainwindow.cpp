#include "mainwindow.h"
#include "segy.h"
#include "tracewidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createToolBars();
    createContextMenus();

    mdiArea = new QMdiArea;
    setCentralWidget(mdiArea);
    resize(640,480);
    XSliceInterval = 1;

    /*operate the database*/
    CdbTrace = new CtrlDB;


}

MainWindow::~MainWindow()
{


}

void MainWindow::createActions()    /*菜单选项*/
{
    /*文件菜单*/
    openAction = new QAction(tr("打开"),this);
    openAction->setStatusTip(tr("打开新文件"));
    connect(openAction,SIGNAL(triggered()),this,SLOT(openFile()));

    importAction = new QAction(tr("导入"),this);
    importAction->setStatusTip(tr("导入数据"));
    connect(importAction,SIGNAL(triggered()),this,SLOT(importFile()));
    importAction->setDisabled(true);

    sortAction = new QAction(tr("分类数据"),this);
    sortAction->setStatusTip(tr("处理分类数据"));
    connect(sortAction,SIGNAL(triggered()),this,SLOT(sortFile()));
    sortAction->setDisabled(true);

    exitAction = new QAction(tr("退出"),this);
    exitAction->setStatusTip(tr("退出程序"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    /*导入菜单*/
    ebdicAction = new QAction(tr("显示EBDIC码"),this);
    ebdicAction->setStatusTip(tr("显示EBDIC码"));
    connect(ebdicAction,SIGNAL(triggered()),this,SLOT(loadEBDIC()));
    ebdicAction->setDisabled(true);

    reelAction = new QAction(tr("显示卷头"),this);
    reelAction->setStatusTip(tr("显示卷头信息"));
    connect(reelAction,SIGNAL(triggered()),this,SLOT(loadReel()));
    reelAction->setDisabled(true);

    traceAction = new QAction(tr("显示道头"),this);
    traceAction->setStatusTip(tr("显示选定位置道头信息"));
    connect(traceAction,SIGNAL(triggered()),this,SLOT(loadTrace()));
    traceAction->setDisabled(true);

    dataAction = new QAction(tr("地震数据"),this);
    dataAction->setStatusTip(tr("显示选定位置的地震记录"));
    connect(dataAction,SIGNAL(triggered()),this,SLOT(loadData()));
    dataAction->setDisabled(true);

    /*画图菜单*/
    waveAction = new QAction(tr("波形显示"),this);
    waveAction->setStatusTip(tr("波形显示剖面"));
    connect(waveAction,SIGNAL(triggered()),this,SLOT(drawWave()));
    waveAction->setDisabled(true);


    grayAction = new QAction(tr("灰度显示"),this);
    grayAction->setStatusTip(tr("显示灰度图像"));
    connect(grayAction,SIGNAL(triggered()),this,SLOT(drawGray()));
    grayAction->setDisabled(true);

    colorAction = new QAction(tr("彩色显示"),this);
    colorAction->setStatusTip(tr("显示彩色图像"));
    connect(colorAction,SIGNAL(triggered()),this,SLOT(drawColor()));
    colorAction->setDisabled(true);

    //操作openGL图像
    bigAction = new QAction(tr("放大"),this);
    bigAction->setStatusTip(tr("放大图像"));
    connect(bigAction,SIGNAL(triggered()),this,SLOT(bigger()));
    bigAction->setDisabled(true);

    littleAction = new QAction(tr("缩小"),this);
    littleAction->setStatusTip(tr("缩小图像"));
    connect(littleAction,SIGNAL(triggered()),this,SLOT(little()));
    littleAction->setDisabled(true);

    moveleftAction = new QAction(tr("左移"),this);
    moveleftAction->setStatusTip(tr("左移图像"));
    connect(moveleftAction,SIGNAL(triggered()),this,SLOT(moveleft()));
    moveleftAction->setDisabled(true);

    moverightAction = new QAction(tr("右移"),this);
    moverightAction->setStatusTip(tr("图像右移"));
    connect(moverightAction,SIGNAL(triggered()),this,SLOT(moveright()));
    moverightAction->setDisabled(true);

    moveupAction = new QAction(tr("上移"),this);
    moveupAction->setStatusTip(tr("上移图像"));
    connect(moveupAction,SIGNAL(triggered()),this,SLOT(moveup()));
    moveupAction->setDisabled(true);

    movedownAction = new QAction(tr("下移"),this);
    movedownAction->setStatusTip(tr("下移图像"));
    connect(movedownAction,SIGNAL(triggered()),this,SLOT(movedown()));
    movedownAction->setDisabled(true);

    moveMouseAction = new QAction(tr("移动"),this);
    moveMouseAction->setStatusTip(tr("使用鼠标移动图像"));
    connect(moveMouseAction,SIGNAL(triggered()),this,SLOT(moveMouse()));
    moveMouseAction->setDisabled(true);

    rotateMouseAction = new QAction(tr("旋转"),this);
    rotateMouseAction->setStatusTip(tr("旋转图像"));
    connect(rotateMouseAction,SIGNAL(triggered()),this,SLOT(rotateMouse()));
    rotateMouseAction->setDisabled(true);

    nextSliceAction = new QAction(tr("下一条剖面"),this);
    nextSliceAction->setStatusTip(tr("选择下一条剖面"));
    connect(nextSliceAction,SIGNAL(triggered()),this,SLOT(nextSlice()));
    nextSliceAction->setDisabled(true);

    preSliceAction = new QAction(tr("上一条剖面"),this);
    preSliceAction->setStatusTip(tr("选择上一条剖面"));
    connect(preSliceAction,SIGNAL(triggered()),this,SLOT(preSlice()));
    preSliceAction->setDisabled(true);

    drawGridAction = new QAction(tr("关闭网格"),this);
    drawGridAction->setStatusTip(tr("打开/关闭剖面图像上的网格"));
    connect(drawGridAction,SIGNAL(triggered()),this,SLOT(drawGrid()));
    drawGridAction->setDisabled(true);
}

void MainWindow::createMenus()      /*添加菜单*/
{
    fileMenu = menuBar()->addMenu(tr("&F文件"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(importAction);
    fileMenu->addAction(sortAction);
    fileMenu->addAction(exitAction);

    loadMenu = menuBar()->addMenu(tr("&D数据"));
    loadMenu->addAction(ebdicAction);
    loadMenu->addAction(reelAction);
    loadMenu->addAction(traceAction);
    loadMenu->addAction(dataAction);

    drawMenu = menuBar()->addMenu(tr("&S显示"));
    drawMenu->addAction(waveAction);
    drawMenu->addAction(grayAction);
    drawMenu->addAction(colorAction);

    mouseFunc = menuBar()->addMenu(tr("查看图像"));
    mouseFunc->addAction(moveMouseAction);
    mouseFunc->addAction(rotateMouseAction);


}

void MainWindow::createToolBars()   /*添加工具栏*/
{
    QToolBar *fileTool = addToolBar(tr("打开文件"));
    fileTool->addAction(openAction);
    fileTool->addAction(importAction);
    fileTool->addAction(sortAction);
    fileTool->addAction(exitAction);

    QToolBar *loadTool = addToolBar(tr("数据"));
    loadTool->addAction(ebdicAction);
    loadTool->addAction(reelAction);
    loadTool->addAction(traceAction);
    loadTool->addAction(dataAction);

    QToolBar *drawTool = addToolBar(tr("显示"));
    drawTool->addAction(waveAction);
    drawTool->addAction(grayAction);
    drawTool->addAction(colorAction);

    spin_interval = new QSpinBox;
    spin_interval->setRange(1,1000);
    spin_interval->setValue(1);
    connect(spin_interval,SIGNAL(valueChanged(int)),this,SLOT(XSliceInterval_Changed(int)));

    spin_result = new QSpinBox;
    spin_result->setRange(1,10000);
    spin_result->setValue(1);
    spin_result->setRange(12,9000);
    connect(spin_result,SIGNAL(valueChanged(int)),this,SLOT(setXSliceNum(int)));

    QToolBar *operationTool = addToolBar(tr("图像操作"));
    operationTool->addAction(bigAction);
    operationTool->addAction(littleAction);
    operationTool->addAction(moveleftAction);
    operationTool->addAction(moverightAction);
    operationTool->addAction(moveupAction);
    operationTool->addAction(movedownAction);
    operationTool->addAction(drawGridAction);
    operationTool->addAction(preSliceAction);
    operationTool->addWidget(spin_interval);
    operationTool->addAction(nextSliceAction);
    operationTool->addWidget(spin_result);

    QToolBar *RotateTool = addToolBar(tr("旋转图像"));
    Xslider = new QSlider(Qt::Horizontal);
    Xslider->setRange(0,360);
    Xslider->setValue(0);
    Xslider->setDisabled(true);
    Xspin = new QSpinBox;
    Xspin->setRange(0,360);
    Xspin->setValue(0);
    Xspin->setDisabled(true);
    connect(Xslider,SIGNAL(valueChanged(int)),this,SLOT(RotateX(int)));
    connect(Xslider,SIGNAL(valueChanged(int)),Xspin,SLOT(setValue(int)));
    connect(Xspin,SIGNAL(valueChanged(int)),Xslider,SLOT(setValue(int)));

    Yslider = new QSlider(Qt::Horizontal);
    Yslider->setRange(0,360);
    Yslider->setValue(0);
    Yslider->setDisabled(true);
    Yspin = new QSpinBox;
    Yspin->setRange(0,360);
    Yspin->setValue(0);
    Yspin->setDisabled(true);
    connect(Yslider,SIGNAL(valueChanged(int)),this,SLOT(RotateY(int)));
    connect(Yslider,SIGNAL(valueChanged(int)),Yspin,SLOT(setValue(int)));
    connect(Yspin,SIGNAL(valueChanged(int)),Yslider,SLOT(setValue(int)));

    Zslider = new QSlider(Qt::Horizontal);
    Zslider->setRange(0,360);
    Zslider->setValue(0);
    Zslider->setDisabled(true);
    Zspin = new QSpinBox;
    Zspin->setRange(0,360);
    Zspin->setValue(0);
    Zspin->setDisabled(true);
    connect(Zslider,SIGNAL(valueChanged(int)),this,SLOT(RotateZ(int)));
    connect(Zslider,SIGNAL(valueChanged(int)),Zspin,SLOT(setValue(int)));
    connect(Zspin,SIGNAL(valueChanged(int)),Zslider,SLOT(setValue(int)));
    RotateTool->addWidget(Xslider);RotateTool->addWidget(Xspin);
    RotateTool->addWidget(Yslider);RotateTool->addWidget(Yspin);
    RotateTool->addWidget(Zslider);RotateTool->addWidget(Zspin);



}

void MainWindow::createContextMenus()/*右键菜单*/
{

}

void MainWindow::openFile()
{    
    filename =tr("");

    filename =  QFileDialog::getOpenFileName(this,
               tr("打开SEGY文件"),tr("/work/SEGY"),tr("SEGY文件 (*.sgy *.segy);; 所有文件 (*.*)"));
    if(filename.isEmpty())
    {
        int r =QMessageBox::warning(this,tr("错误"),tr("输入文件名为空"),QMessageBox::Yes| QMessageBox::No);
        if( r == QMessageBox::Yes )
            emit openFile();
        else
            emit close();
    }
    bool isOK;
    QStringList item;
    item<<tr("二维数据体")<<tr("三维数据体");
    CubeType = QInputDialog::getItem(this,tr("选择数据类型"),tr("选择数据类型"),item,0,true,&isOK);
    //*active actions*//
    importAction->setEnabled(true);
    sortAction->setEnabled(true);
    ebdicAction->setEnabled(true);



    //*Active Actions*//

}

void MainWindow::importFile()
{
    QFile loadfile(filename);
    loadfile.open(QIODevice::ReadOnly);
    QDataStream ls(&loadfile);
    ls.setByteOrder(QDataStream::BigEndian);
    ls.device()->seek(3200);
    Reel.ReadREEL(ls);

    //Max_XLine=Max_YLine=Min_XLine=Min_YLine=0;

    DBTrace t;


    int xLine;               //主测线号XLINE
    int oldxLine;
    int inLine;              //inLine 号
    int temp,_temp,N;
    temp =_temp=0;
    N=0;

    QFile writefile(filename+tr("_db"));
    writefile.open(QIODevice::WriteOnly);
    QDataStream ws(&writefile);
    QProgressDialog *pdg = new QProgressDialog(this);
    QFont font("Times",10,QFont::Bold);
    pdg->setFont(font);
    pdg->setWindowModality(Qt::WindowModal);
    pdg->setWindowTitle(tr("导入SEGY文件"));
    pdg->setLabelText(tr("扫描SEGY数据"));
    pdg->setCancelButtonText(tr("取消"));
    pdg->setRange(0,Reel.TotalTraces);
    pdg->setMinimumDuration(500);
    int HNS = Reel.hns;
    int DataType = Reel.DataType;
    xLine = oldxLine = 0;
    pdg->setValue(0);
    qApp->processEvents();

    for( int i = 0; i < Reel.TotalTraces; i ++)
    {
        double fp =  3600.0 + 8.0 + double(i)*double(HNS*DataType + 240.0);
        pdg->setValue(i+1);
        qApp->processEvents();
        if (pdg->wasCanceled())
        {
            break;
        }
        ws<<i;
        ls.device()->seek(fp);
        ls >>xLine;
        ls.device()->seek(fp+12.0);
        ls>>inLine;

        t.fp = i;
        t.in = xLine;
        t.cross = inLine;
        t.st = 0;
        t.et = Reel.hns*Reel.hdt/1000;
        CdbTrace->AddData(t);

        if( i == 0 )
        {
            Max_XLine = xLine;
            Min_XLine = xLine;
            Max_YLine = inLine;
            Min_YLine = inLine;
        }
        else
        {
            if( xLine > Max_XLine )
                Max_XLine = xLine;
            if( xLine < Min_XLine )
                Min_XLine = xLine;
            if( inLine > Max_YLine )
                Max_YLine = inLine;
            if( inLine < Min_YLine )
                Min_YLine = inLine;
        }

    }

    loadfile.close();


    /**/
    reelAction->setEnabled(true);
    traceAction->setEnabled(true);
    dataAction->setEnabled(true);
}

void MainWindow::sortFile()
{

}

void MainWindow::loadEBDIC()
{
    txtEdit= new QTextEdit();//setCentralWidget(t);
    txtEdit->setWindowTitle(tr("EBDIC码"));
    mdiArea->addSubWindow(txtEdit);
    txtEdit->show();
    //txtEdit->setText(tr("显示EBDIC码："));
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QDataStream EBfile(&file);
    QString str = tr("");
    unsigned char temp = NULL;
    for( int i = 0; i < 40; i ++)
    {
        for( int j = 0; j < 80; j++)
        {
            EBfile>>temp;
            temp=EBCDICtoASCII(temp);
            //str += QString::fromAscii((const char*)&temp,1);
            str += QString(temp);
        }
        str += tr("\n");
    }
    txtEdit->append(str);
    file.close();
}

void MainWindow::loadData()
{
    /*激活菜单*/
    waveAction->setEnabled(true);
    grayAction->setEnabled(true);
    colorAction->setEnabled(true);
    rotateMouseAction->setEnabled(true);
    moveMouseAction->setEnabled(true);
    movedownAction->setEnabled(true);
    moveupAction->setEnabled(true);
    moverightAction->setEnabled(true);
    moveleftAction->setEnabled(true);
    littleAction->setEnabled(true);
    bigAction->setEnabled(true);
    drawGridAction->setEnabled(true);
    Xslider->setEnabled(true);
    Yslider->setEnabled(true);
    Zslider->setEnabled(true);
    Xspin->setEnabled(true);
    Yspin->setEnabled(true);
    Zspin->setEnabled(true);
    /********/
    XSliceNum = 1;
    wgl = new GLWidget(this);
    mdiArea->addSubWindow(wgl);
    wgl->Name =filename;
    wgl->MReel =Reel;

    wgl->SetData();
    wgl->isDraw =true;
    wgl->show();

    spin_result->setRange(Min_XLine,Max_XLine-1);
    spin_result->setValue(Min_XLine);
    XSliceNum = Min_XLine;
    setXSliceNum(XSliceNum);
    //wgl->MReel = Reel;

    //wgl->MyDraw();

}
void MainWindow::loadReel()
{
    QString myReelName[]=
    {
        tr("任务识别码"),
        tr("测线号"),
        tr("卷号"),
        tr("每个记录的道数"),
        tr("每个记录的辅助道数"),
        tr("该卷采样间隔(微秒)"),
        tr("原始记录的采样间隔(微秒)"),
        tr("该卷每道的样点数"),
        tr("原始记录的每道的采样点"),
        tr("数据采样点的记录格式"),
        tr("覆盖次数"),
        tr("道分选码"),
        tr("垂直和码"),
        tr("起始扫描频率"),
        tr("终止扫描频率"),
        tr("扫描时窗"),
        tr("扫描类型"),
        tr("扫描频带道数"),
        tr("斜坡扫描起始时间"),
        tr("斜坡扫描终止时间"),
        tr("斜坡扫描类型"),
        tr("相关数据道"),
        tr("增益恢复"),
        tr("振幅恢复方法"),
        tr("测量系统"),
        tr("激发信号极性"),
        tr("振动极性"),
        tr("总道数")
    };
    int ReelALL[28];
    std::vector<int> temvec;
    temvec.push_back(Reel.jobid);
    temvec.push_back(Reel.lino);
    temvec.push_back(Reel.reno);
    temvec.push_back(Reel.ntrpr);
    temvec.push_back(Reel.nart);
    temvec.push_back(Reel.hdt);
    temvec.push_back(Reel.dto);
    temvec.push_back(Reel.hns);
    temvec.push_back(Reel.nso);
    temvec.push_back(Reel.format);
    temvec.push_back(Reel.fold);
    temvec.push_back(Reel.tsort);
    temvec.push_back(Reel.hsfs);
    temvec.push_back(Reel.vscode);
    temvec.push_back(Reel.hsfe);
    temvec.push_back(Reel.hslen);
    temvec.push_back(Reel.hstyp);
    temvec.push_back(Reel.schn);
    temvec.push_back(Reel.hstas);
    temvec.push_back(Reel.hstae);
    temvec.push_back(Reel.htatyp);
    temvec.push_back(Reel.hcorr);
    temvec.push_back(Reel.bgrcv);
    temvec.push_back(Reel.rcvm);
    temvec.push_back(Reel.mfeet);
    temvec.push_back(Reel.vpol);
    temvec.push_back(Reel.DataType);
    temvec.push_back(Reel.TotalTraces);

    for(int i = 0; i < 28; i++)
        ReelALL[i]=temvec[i];
    temvec.clear();
    std::vector<int>().swap(temvec);

       table = new QTableWidget(28,2,this);//setCentralWidget(table);
       table->setMaximumWidth(250);
       QStringList headers;
       headers<<tr("记录名称")<<tr("记录值");
       table->setHorizontalHeaderLabels(headers);
       mdiArea->addSubWindow(table);

       table->show();
       //QTextEdit* t= new QTextEdit();setCentralWidget(t);

       for( int i = 0; i < 28; i ++ )
       {
           QTableWidgetItem* item1 = new QTableWidgetItem();
           //item->setFlags(item->flags() &(Qt::ItemIsEditable & Qt::ItemIsSelectable));
           item1->setText(myReelName[i]);
           table->setItem(i,0,item1);

           QTableWidgetItem* item2 = new QTableWidgetItem();
           item2->setText(QString::number(ReelALL[i]));
           table->setItem(i,1,item2);
       }

}

void MainWindow::loadTrace()
{
    tracewidget = new TraceWidget(this);
    mdiArea->addSubWindow(tracewidget);
    tracewidget->show();

}
/*******************************/

void MainWindow::drawWave()
{
    wgl->changeSectionType(0);
}

void MainWindow::drawGray()
{
    wgl->changeSectionType(1);

}
void MainWindow::RotateX(int X)
{
    wgl->Use_RotateX(X);
}
void MainWindow::RotateY(int Y)
{
    wgl->Use_RotateY(Y);
}
void MainWindow::RotateZ(int Z)
{
    wgl->Use_RotateZ(Z);
}

void MainWindow::drawColor()
{
    wgl->changeSectionType(2);
}
void MainWindow::bigger()
{
    wgl->bigger();
}
void MainWindow::little()
{
    wgl->little();
}
void MainWindow::moveleft()
{
    wgl->moveleft();
}
void MainWindow::moveright()
{
    wgl->moveright();
}
void MainWindow::moveup()
{
    wgl->moveup();
}
void MainWindow::movedown()
{
    wgl->movedown();
}
void MainWindow::moveMouse()
{
    wgl->Chang_Mouse_Type(0);
}
void MainWindow::rotateMouse()
{
    wgl->Chang_Mouse_Type(1);
}
void MainWindow::nextSlice()
{
    XSliceNum += XSliceInterval;
    if ( XSliceNum >= Max_XLine-1 )
    {
        nextSliceAction->setDisabled(true);
        XSliceNum = Max_XLine-1;
    }
    spin_result->setValue(XSliceNum);
    setXSliceNum(XSliceNum);
    preSliceAction->setEnabled(true);
}
void MainWindow::preSlice()
{
    XSliceNum -= XSliceInterval;
    if ( XSliceNum <= Min_XLine )
    {
        preSliceAction->setDisabled(true);
        XSliceNum = Min_XLine;
    }
    spin_result->setValue(XSliceNum);
    setXSliceNum(XSliceNum);
    nextSliceAction->setEnabled(true);
}
void MainWindow::setYSliceNum(int Num)
{
    int N = Max_YLine - Min_YLine;
    int fp[N];
    memset(&fp,0,N);
    int j = 0;
    for( int i = 0; i < Reel.TotalTraces; i++)
    {
        if(YLine[i] == Num)
        {
            fp[j] = i;
            j ++;
        }
    }
}
void MainWindow::setTimeSliceType(int num)
{

}

void MainWindow::setXSliceNum(int sliceNum )
{
    QFile dbfile(filename+tr("_Xdb"));
    dbfile.open(QIODevice::ReadOnly);
    QDataStream DBin(&dbfile);

    sliceNum -= (Min_XLine-1);
    int fpCount,traceNum,LineNum;
    if( CubeType == tr("二维数据体"))
    {
        DBin>>fpCount>>fpCount>>traceNum>>LineNum;
        fpCount += 3840;
        wgl->setInLine(fpCount,traceNum,LineNum);
    }
    else if( CubeType == tr("三维数据体"))
    {
        nextSliceAction->setEnabled(true);
        DBin.device()->seek(( sliceNum )*(3*sizeof(int)));
        DBin>>fpCount;
        DBin>>traceNum;
        DBin>>LineNum;
        qint64 fp =3840+(fpCount-traceNum)*(Reel.DataType*Reel.hns + 240);
        wgl->setInLine(fp,traceNum,LineNum);
    }

}
void MainWindow::XSliceInterval_Changed(int interval)
{
    XSliceInterval = interval;
}
void MainWindow::drawGrid()
{
    if(!wgl->isDrawGrid)
    {
        drawGridAction->setText(tr("关闭网格"));
    }
    else
    {
        drawGridAction->setText(tr("打开网格"));
    }
    wgl->DrawGrid();
}

/*******************************/

