#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <QList>
#include <QVector>
#include <QColor>
#include <QSettings>

#include "../DataBase/database.h"
#include "../SEGY/showslice.h"

class ShowTrace;
class ShowSlice;

struct REEL;
struct TRACE;
struct SEGYHEAD;
struct Surface
{
    int x,y,z;
    float value;
};

enum SeisType{WaveLine,WaveArea,Gray,Colors};

struct MapSetting
{
    //底图参数设置
    int Min_XLine,Max_XLine;    //XLine的最大最小值
    int Min_YLine,Max_YLine;    //YLine的最大最小值
    int XInterval,YInterval;    //X,YLine的各道之间的间隔
    float XScale,YScale;        //X,Y方向上的缩放比例
    int size_Receiver;          //检波点大小，选择半径
    QColor color_Receiver;      //检波点颜色
    QColor color_Receiver_Selected; //选择到的检波点的颜色
};

struct SeisSetting
{
    //剖面参数设置
    float dx,dy;                //X方向上的间隔和Z时间上的间隔
    float sample_interval;      //时间采样间隔
    float XScale,YScale;        //剖面上X方向和T方向的缩放比例
    float gain;                 //数据增益

    int lx,ly;                  //距离左边界和上边界的间隔
    int grid_XInterval,grid_YInterval;//背景网格线的间距
    QColor color_grid;          //网格划线的颜色
    SeisType seisType;          //地震剖面绘制方式
    QColor color_Line;          //剖面划线的颜色
    int width_Line;             //剖面划线的线宽
    QColor color_Area;          //波形变面积的填充颜色
};

class DataManager
{
public:
    DataManager();
    ~DataManager();
    void ReadSEGYData();
    void GetSurface(int ztime);

    void setMemory();       //声明数组空间
    bool setDataBase();     //链接数据库
    void setDefaultValues();//设置默认值s
    bool InitFile();

    //读写项目配置文件
    void ReadSetting();
    void WriteSetting();

public:
    QSqlQuery query;
    QString filename;   //地震数据文件
    QString dbname;        //数据库文件
    FILE* filesegy;

    REEL* Reel;     //地震数据卷头信息
    SEGYHEAD *segyHead;//地震数据道头信息


    //地震数据
    QList<Surface> surface;
    QList<ShowSlice*> Total_Slices;
    int Current_Slice_Index;
    //ShowSlice* Current_Slice;   //当前显示的地震剖面

    //绘图设置
    MapSetting  *mapset;
    SeisSetting *seisset;

};

#endif // DATAMANAGER_H
