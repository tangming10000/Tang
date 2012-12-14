#include "tracewidget.h"
#include "segy.h"
#include <QFile>
#include <QDataStream>

TraceWidget::TraceWidget(MainWindow *parent)
    : QWidget(parent)
{
    mainWindow = parent;
    name = mainWindow->filename;
    Reel = mainWindow->Reel;
    fp =1;
    setMinimumSize(640,480);
    slider = new QSlider(Qt::Horizontal);
    spinBox = new QSpinBox;
    spinBox->setRange(1,Reel.TotalTraces);
    slider->setRange(1,Reel.TotalTraces);

    QHBoxLayout* toplayout = new QHBoxLayout;
    toplayout->addWidget(slider);
    toplayout->addWidget(spinBox);

    table = new QTableWidget(this);
    tableData = new QTableWidget(this);
    DWidget = new TraceWidgetDraw(this);
    table->setMaximumWidth(250);
    tableData->setMaximumWidth(200);
    //DWidget->setMinimumWidth(this->geometry().width()-500);
    QHBoxLayout *tableLayout = new QHBoxLayout;
    tableLayout->addWidget(table);
    tableLayout->addWidget(tableData);
    tableLayout->addWidget(DWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(toplayout);
    layout->addLayout(tableLayout);
    setLayout(layout);


    connect(spinBox,SIGNAL(valueChanged(int)),slider,SLOT(setValue(int)));
    connect(slider,SIGNAL(valueChanged(int)),spinBox,SLOT(setValue(int)));
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(ShowTrace(int)));
    spinBox->setValue(1);

    table->setRowCount(71);
    table->setColumnCount(2);
    QStringList headers1;
    headers1<<tr("记录名称")<<tr("记录值");
    table->setHorizontalHeaderLabels(headers1);

    tableData->setRowCount(Reel.hns);
    tableData->setColumnCount(1);
    QStringList headers;
    headers<<tr("振幅值/时间(%1us)").arg(QString::number(Reel.hdt));
    tableData->setHorizontalHeaderLabels(headers);

}

TraceWidget::~TraceWidget()
{

}
void TraceWidget::resizeEvent(QResizeEvent *)
{
    DWidget->setMinimumWidth(this->geometry().width()-table->width()-tableData->width()-50);
}

void TraceWidget::ShowTrace(int tfp)
{
   //Trace.ReadTable((const char*)filename.toLocal8Bit(),fp,Reel.hns,Reel.DataType);
    //reset();
    QString TraceName[]={
        tr("测线中道顺序号"),
        tr("磁带中道顺序号"),
         tr("原始野外记录号"),
         tr("原始野外记录中的道号"),
         tr("震源点号"),
         tr("CDP 号"),
         tr("CDP 道集中的道号"),
         tr("道鉴别码:"),
         tr("垂直叠加道数"),
         tr("水平叠加道数"),
         tr("数据类型:"),
         tr("炮检距"),
         tr("接收点高程"),
         tr("炮点的地面高程"),
         tr("炮点低于地面的深度"),
         tr("接收点的基准面高程"),
         tr("炮点的基准面高程"),
         tr("炮点的水深"),
         tr("接收点的水深"),
         tr("前七个比例因子"),
         tr("前四个比例因子"),
         tr("炮点 X 坐标"),
         tr("炮点 Y 坐标"),
         tr("检波点 X 坐标"),
         tr("检波点 Y 坐标"),
         tr("坐标单位"),
         tr("风化层速度"),
         tr("降速层速度"),
         tr("震源处的井口时间"),
         tr("接收点处的井口时间"),
         tr("炮点的静校正"),
         tr("接收点的静校正"),
         tr("应用的总静校正"),
         tr("延迟时间 A"),
         tr("延迟时间 B"),
         tr("延迟记录时间"),
         tr("起始切除时间"),
         tr("终止切除时间"),
         tr("本道采样点数"),
         tr("本道采样间隔"),
         tr("仪器的增益类型"),
         tr("仪器的固定增益"),
         tr("仪器的原始增益"),
         tr("相关码"),
         tr("扫描起始频率"),
         tr("扫描终止频率"),
         tr("扫描长度(ms)"),
         tr("扫描类型"),
         tr("扫描道起始斜坡长度"),
         tr("扫描道终止斜坡长度"),
         tr("扫描斜坡类"),
         tr("假频频率"),
         tr("假频滤波器斜坡"),
         tr("陷波滤波器频率"),
         tr("陷波滤波器斜坡"),
         tr("低截频"),
         tr("高截频"),
         tr("低截滤波器斜坡"),
         tr("高截滤波器斜坡"),
         tr("年"),
         tr("日期"),
         tr("小时"),
         tr("分"),
         tr("秒"),
         tr("时间代码"),
         tr("道加权因子"),
         tr("覆盖开关位置 1 的检波器道号"),
         tr("在原始野外记录中道号 1 的检波点号"),
         tr("原始野外记录中最后一道的检波点号"),
         tr("缺口大小"),
         tr("在测线开始或者结束处的斜坡位置")
        };

    QFile file(name);
    file.open(QIODevice::ReadOnly);
    QDataStream inss(&file);
    inss.setByteOrder(QDataStream::BigEndian);

    tfp -= 1;

    inss.device()->seek(3600+tfp*(Reel.hns*Reel.DataType + 240));
    Trace.ReadTRACE(inss);
    output();

    for( int i = 0; i < 71; i++)
    {
        QTableWidgetItem* item0 = new QTableWidgetItem();
        //item->setFlags(item->flags() &(Qt::ItemIsEditable & Qt::ItemIsSelectable));
        item0->setText(TraceName[i]);
        table->setItem(i,0,item0);

        QTableWidgetItem* item1 = new QTableWidgetItem();
        //item->setFlags(item->flags() &(Qt::ItemIsEditable & Qt::ItemIsSelectable));
        item1->setText(QString::number(TraceAll[i]));
        table->setItem(i,1,item1);
    }


    float Data[Reel.hns];
    DWidget->N=Reel.hns;

    ReadDataATP(inss,Data,Reel.hns,Reel.format);
    for(int i = 0; i<Reel.hns;i++)
    {
        QTableWidgetItem* item1 = new QTableWidgetItem();
        item1->setText(QString::number(Data[i]));
        tableData->setItem(i,0,item1);
    }
    DWidget->SetData(Data,Reel.hns);
}

void TraceWidget::output()
{
    std::vector<int> temvec;
    temvec.push_back(Trace.tracl);
    temvec.push_back(Trace.tracr);
    temvec.push_back(Trace.fldr);
    temvec.push_back(Trace.tracf);
    temvec.push_back(Trace.ep);
    temvec.push_back(Trace.cdp);
    temvec.push_back(Trace.cdpt);
    temvec.push_back(Trace.trid);
    temvec.push_back(Trace.nvs);
    temvec.push_back(Trace.nhs);
    temvec.push_back(Trace.duse);
    temvec.push_back(Trace.offset);
    temvec.push_back(Trace.gelev);
    temvec.push_back(Trace.selev);
    temvec.push_back(Trace.sdepth);
    temvec.push_back(Trace.gdel);
    temvec.push_back(Trace.sdel);
    temvec.push_back(Trace.swdep);
    temvec.push_back(Trace.gwdep);
    temvec.push_back(Trace.scalel);
    temvec.push_back(Trace.scalco);
    temvec.push_back(Trace.sx);
    temvec.push_back(Trace.sy);
    temvec.push_back(Trace.gx);
    temvec.push_back(Trace.gy);
    temvec.push_back(Trace.counit);
    temvec.push_back(Trace.wevel);
    temvec.push_back(Trace.swevel);
    temvec.push_back(Trace.sut);
    temvec.push_back(Trace.gut);
    temvec.push_back(Trace.sstat);
    temvec.push_back(Trace.gstat);
    temvec.push_back(Trace.tstat);
    temvec.push_back(Trace.laga);
    temvec.push_back(Trace.lagb);
    temvec.push_back(Trace.delrt);
    temvec.push_back(Trace.muts);
    temvec.push_back(Trace.mute);
    temvec.push_back(Trace.ns);
    temvec.push_back(Trace.dt);
    temvec.push_back(Trace.gain);
    temvec.push_back(Trace.igc);
    temvec.push_back(Trace.igi);
    temvec.push_back(Trace.corr);
    temvec.push_back(Trace.sfs);
    temvec.push_back(Trace.sfe);
    temvec.push_back(Trace.slen);
    temvec.push_back(Trace.styp);
    temvec.push_back(Trace.stas);
    temvec.push_back(Trace.stae);
    temvec.push_back(Trace.tatyp);
    temvec.push_back(Trace.afilf);
    temvec.push_back(Trace.afils);
    temvec.push_back(Trace.nofilf);
    temvec.push_back(Trace.nofils);
    temvec.push_back(Trace.lcf);
    temvec.push_back(Trace.hcf);
    temvec.push_back(Trace.lcs);
    temvec.push_back(Trace.hcs);
    temvec.push_back(Trace.year);
    temvec.push_back(Trace.day);
    temvec.push_back(Trace.hour);
    temvec.push_back(Trace.minute);
    temvec.push_back(Trace.sec);
    temvec.push_back(Trace.timbas);
    temvec.push_back(Trace.trwf);
    temvec.push_back(Trace.grnors);
    temvec.push_back(Trace.grnofr);
    temvec.push_back(Trace.grnlof);
    temvec.push_back(Trace.gaps);
    temvec.push_back(Trace.otrav);
    for( int i =0; i < 71; i++)
        TraceAll[i]=temvec[i];
    temvec.clear();
    std::vector<int>().swap(temvec);

}
