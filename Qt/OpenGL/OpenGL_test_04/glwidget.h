#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QtGui>
#include <QTimer>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#ifdef Q_WS_X11
#include <GL/glx.h>
#endif
#define GL_FOG_COORDINATE_SOURCE_EXT	0x8450					// 从GLEXT.H得到的值
#define GL_FOG_COORDINATE_EXT		0x8451
typedef void (APIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);		// 声明雾坐标函数的原形

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
    bool getExtensions();
    void setLight();//设置光照
    void setFog();  //设置雾
    void setRotate();
    void Go_init();     //回到初始状态
    void Draw_Arrows();
    void Draw();
    void VolumeFog();

//Define the global variables of the program
private:
    PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
    GLfloat xRot,yRot,zRot,zoom;
    GLfloat xSpeed,ySpeed;
    GLfloat scale;
    QPoint lasPos;
    int t;

    GLuint theTorus;
};

#endif // GLWIDGET_H
