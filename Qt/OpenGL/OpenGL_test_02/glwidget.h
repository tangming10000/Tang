#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui/QWidget>
#include <QTimer>
#include <QtOpenGL/QtOpenGL>
#include <QtGui>

#define GEARSNUM 10

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
    void Go_init();     //回到初始状态
    void Draw_Arrows();
    void Draw();

    //functions
    void setLists();
    void Gear(GLfloat inner_radius, GLfloat outer_radius,GLfloat w,GLfloat teeth,GLfloat tooth_depth);
    GLfloat angle;
    GLfloat teethnum;



//Define the global variables of the program
private:
    GLfloat xRot,yRot,zRot,zoom;
    GLfloat xSpeed,ySpeed;
    GLfloat scale;
    QPoint lasPos;
    int t;

    //const static int GEARSNUM = 10;
    GLuint gear[GEARSNUM+1];


};

#endif // GLWIDGET_H
