#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QtGui>
#include <QTimer>
#include <QtOpenGL>
#include <cstdlib>
#include <cmath>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>

typedef struct
{
    bool isActive;
    float life;
    float fade;
    float r, g, b;
    float x, y, z;
    float xi,yi,zi;
    float xg,yg,zg;
}Particles;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

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
    void Go_init();     //回到初始状态
    void Draw_Arrows();
    void Draw();

    void InitPar();
    void drawPar();

//Define the global variables of the program
private:
    GLfloat xRot,yRot,zRot,zoom;
    GLfloat xSpeed,ySpeed;
    GLfloat scale;
    QPoint lasPos;
    int t;

    //set for particles
    int MAX_PARTICLES;
    bool isRainbow;
    float slowdown;
    GLuint loop,col,delay;
    Particles *PP;




};

#endif // GLWIDGET_H
