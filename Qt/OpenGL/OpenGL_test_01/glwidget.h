#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui/QWidget>
#include <QTimer>
#include <QtOpenGL/QtOpenGL>
#include <GL/glu.h>
#include <QtGui>

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
    void setRotate();
    void Go_init();     //»Øµ½³õÊ¼×´Ì¬
    void Draw_Arrows();
    void Draw();
    void cube(GLfloat length);
    void FillTorus(float rc, int numc, float rt, int numt);
    void DisplayList();

//Define the global variables of the program
protected:
    float xRot,yRot,zRot,zoom;
    float xSpeed,ySpeed;
    float scale;
    QPoint lasPos;
    int t;

    GLuint theTorus;
};

#endif // GLWIDGET_H
