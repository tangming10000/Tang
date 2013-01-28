#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QtGui>
#include <QTimer>
#include <cstdlib>
#include <cmath>
#ifdef WIN32
#include <QtOpenGL>
//#include <gl/GL.h>
#include <gl/GLU.h>
#else
#include <Qt/QtOpenGL>
#include <GL/glext.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include "../../mainwindow.h"


class DataManager;
class MainWindow;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(MainWindow *parent = 0);
    ~GLWidget();
    void SetData(DataManager *DM);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    //The timer,Keyboard and mouse event
    void timerEvent(QTimerEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    void setLight();//设置光照
    void setRotate();

    void DrawCube();  //绘制矩形框
    void Draw_Arrows();
    void Draw();

    void getColor(float value);
    void Draw_SeisColor();  //绘制彩色图像
    void Draw_TraceColor(ShowTrace *t1, ShowTrace *t2);
    void Draw_Surface();


//Define the global variables of the program
public:
    void Go_init();     //回到初始状态
    GLfloat Cube_Width,Cube_Height,Cube_Length;
    GLfloat xRot,yRot,zRot,zoom;
    GLfloat xSpeed,ySpeed;
    GLfloat scale;
    QPoint lasPos;
    GLfloat dx,dt;
    int XMin,YMin;
    int t;
    QList<GLuint> ListNum;
    int Total_ListNum;
    GLfloat color[3];
    DataManager *dm;


};

#endif // GLWIDGET_H
