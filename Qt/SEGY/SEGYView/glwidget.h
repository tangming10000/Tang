#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "segy.h"
#include "mainwindow.h"
#include <cmath>
#include <vector>
#include <QWidget>
#include <QtOpenGL/QtOpenGL>
#include <QtOpenGL/QGLColormap>
#include <GL/glu.h>
#include <QFile>
#include <QDataStream>
#include <QtEvents>
#include <QTimer>
#include <QTimerEvent>
#include <QTextCodec>
using namespace std;
class REEL;
class MainWindow;
struct ShowTrace
{
    int x;
    int y;
    int st;
    int et;
    //float Data[et-st];
};


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(MainWindow *parent );
    ~GLWidget();
    void initializeGL();
    void resizeGL( int w, int h );
    void paintGL();
    void Draw();
    void Draw3D();  //3D地震数据显示
    void Draw2D();  //2D地震数据显示
    void SetData();

private:/*键盘鼠标操作响应函数*/
    void keyPressEvent(QKeyEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);

private:
    void XLineSlice();  /*主测线剖面*/
    void YLineSlice(int CrossLine, int fpointer[],int Num);  /*纵测线剖面,CrossLine CrossLine号，
                                                            剖面的位置指针数组,Num数组大小*/
    void TimeSlice();   /*时间剖面*/

    void draw_wave(float Data[],float Max, int N,int x);/*Data输入数据,Max该道数据中的最大值,N数据个数,x剖面上该道的横坐标
                                                          主要是利用该函数画出波形变面积剖面*/
    void draw_gray(float Data[],float Max,float Min, int N, int x);/*Data输入数据,Max该道数据中的最大值,N数据个数,x剖面上该道的横坐标
                                                          主要是利用该函数画出灰度剖面*/
    void draw_color(float Data[],float Max,float Min, int N, int x);/*Data输入数据,Max该道数据中的最大值,N数据个数,x剖面上该道的横坐标
                                                          主要是利用该函数画出彩色剖面*/

    void AddData( vector<ShowTrace> data);
    void DeleteData(vector<ShowTrace> data);
public:
    //MainWindow* mainWindow;
    REEL MReel;         //卷头信息
    QPoint lasPos;      //鼠标操作
    GLfloat mx,my,zoom; //观测点位置
    QString Name;       //接受文件名
    float PointSize;    //点大小
    bool isDraw;        //画图
    bool isDrawGrid;    //画网格
private:
    vector< vector<ShowTrace> > GLData;
    float** DATA;       //该剖面上的全部地震数据
    GLfloat x_slice_pos; //XLine剖面在立方体上的位置
    //全局变化量
    int DrawType;       //剖面显示方式
    int mouseUse;       //鼠标作用
    MainWindow *main;   //父窗口Mainwindow
    double TotalTraces; //总道数
    int DataType;       //数据格式,每个数据所占字节数
    double sx;              //X坐标间隔
    double st;              //t时间间隔
    int Num;                //该剖面的总道数
    qint64 Fp;              //文件指针的位置
    int LNO;                //剖面的线号
    float xRot,yRot,zRot;   //旋转中心轴


public slots:
    void setInLine(qint64 fp,int TraceNum,int LineNO);  //设置InLine号
    void bigger();      //放大图像
    void little();      //缩小图像
    void moveleft();    //图像左移
    void moveright();   //图像右移
    void moveup();      //图像上移
    void movedown();    //图像下移
    void DrawGrid();    //添加图像网格
    void changeSectionType(int sectiontype);    //选择剖面显示方式
    void Chang_Mouse_Type(int mouseType);       //选择鼠标移动的功能
                        //旋转GLWidget图像
    void Use_RotateX(int);
    void Use_RotateY(int);
    void Use_RotateZ(int);

};    //*其他函数*//


#endif // GLWIDGET_H
