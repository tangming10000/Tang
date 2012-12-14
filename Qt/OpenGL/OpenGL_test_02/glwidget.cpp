#include "glwidget.h"
#include <QVBoxLayout>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setWindowTitle("openGL test_02");
    resize(400,400);
    makeCurrent();
    xRot=yRot=zRot=0.0;
    xSpeed = ySpeed = 0.0;
    scale = 1.0;
    zoom = -10.0;
    angle = 0.0;
    teethnum = 20;

}

GLWidget::~GLWidget()
{
    makeCurrent();
}

void GLWidget::initializeGL()
{
    //setLists();
    qglClearColor(Qt::black);
    glClearDepth(1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    t = startTimer(50);
}

void GLWidget::resizeGL(int w, int h)
{
    if( h == 0 )
        h = 1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(w)/h;
    //glFrustum(-x,x,-1.0,1.0,0.0,10.0);
    gluPerspective( 45.0, x, 0.01,100.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Draw_Arrows();
    //setRotate();
    //Draw();
    swapBuffers();
}

void GLWidget::setRotate()
{
    glLoadIdentity();
    glTranslatef(0.0,0.0,zoom);
    glRotatef(xRot,0.0,0.0,1.0);
    glRotatef(yRot,1.0,0.0,0.0);
    glRotatef(zRot,0.0,1.0,0.0);

    glScalef(scale,scale,scale);
}

void GLWidget::Draw_Arrows()
{
    glLoadIdentity();
    glTranslatef(-0.9,-0.6,-2.0);
    glRotatef(xRot,0.0,0.0,1.0);
    glRotatef(yRot,1.0,0.0,0.0);
    glRotatef(zRot,0.0,1.0,0.0);

    setFont(QFont("Times",12));
    glLineWidth(4);
    glEnable(GL_LINE_SMOOTH);

    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.1,0.0,0.0);
    glEnd();
    renderText(0.2,0.0,0.0,QChar('y'));

    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,0.1,0.0);
    glEnd();
    renderText(0.0,0.2,0.0,QChar('z'));

    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,0.0,0.1);
    glEnd();
    renderText(0.0,0.0,0.2,QChar('x'));

    glDisable(GL_LINE_SMOOTH);

 }

void GLWidget::Draw()
{
    glPushMatrix();
    for( int i = 1; i <= GEARSNUM; i++)
    {
        glPushMatrix();;
        glTranslatef(i*4.2-9.0,0.0,0.0);
        if ( i % 2 == 1)
            glRotatef(angle-90/teethnum,0.0,0.0,1.0);
        else
            glRotatef(-angle,0.0,0.0,1.0);
        glCallList(gear[i]);
        glPopMatrix();
    }
    glPopMatrix();
}

void GLWidget::setLists()
{
    static GLfloat pos[4]={5.0,5.5,10.0,0.0};
    static GLfloat color[4]={0.8,0.0,0.2,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,pos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    for( int i = 1; i <= GEARSNUM; i++)
    {
        gear[i]=glGenLists(i);
        glNewList(gear[i],GL_COMPILE);
            glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color);
            Gear(0.6,2.0,1.0,teethnum,0.7);
        glEndList();
    }
    glEnable(GL_NORMALIZE);
}

void GLWidget::Gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat w, GLfloat teeth, GLfloat tooth_depth)
{
    GLint i;
    GLfloat PI = 3.1415926535;
    GLfloat r0,r1,r2;
    GLfloat angle0,da;
    GLfloat u,v,len;
    r0 = inner_radius;
    r1 = (float)(outer_radius - tooth_depth/2.0);
    r2 = (float)(outer_radius + tooth_depth/2.0);
    da = (float)(2.0*PI/teeth/4.0);
    glShadeModel(GL_SMOOTH);
    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_QUAD_STRIP);
      for( i = 0; i <= teeth; i++)
      {
          angle0 = (float)(i*2.0*PI/teeth);
          if( i > 2&& i < teeth-2)
          {
              glVertex3f((float)(r0*cos(angle0)), (float)(r0*sin(angle0)), (float)w*0.5);
              glVertex3f((float)(r1*cos(angle0)), (float)(r1*sin(angle0)), (float)w*0.5);
              glVertex3f((float)(r0*cos(angle0)), (float)(r0*sin(angle0)), (float)w*0.5);
              glVertex3f((float)(r1*cos(angle0)+3.0*da), (float)(r1*sin(angle0)+3.0*da), (float)w*0.5);
          }
          else
          {
              glVertex3f((float)(r0*1.2), (float)(r0*sin(angle0)), (float)w*0.5);
              glVertex3f((float)(r1*cos(angle0)), (float)(r1*sin(angle0)), (float)w*0.5);
              glVertex3f((float)(r0*1.2), (float)(r0*sin(angle0)), (float)w*0.5);
              glVertex3f((float)(r1*cos(angle0)+3.0*da), (float)(r1*sin(angle0)+3.0*da), (float)w*0.5);
          }
      }
    glEnd();

    /*Draw the Front of the teeth*/
    glBegin(GL_QUADS);
    da = (float)(2.0*PI/teeth/4.0);
    for( i = 0; i < teeth; i++)
    {
        angle0 = (float)(i*2.0*PI/teeth);
        glVertex3f((float)(r1*cos(angle0)), (float)(r1*sin(angle0)), (float)w*0.5);
        glVertex3f((float)(r2*cos(angle0+da)), (float)(r2*sin(angle0+da)), (float)(w*0.5));
        glVertex3f((float)(r2*cos(angle0+2*da)), (float)(r2*sin(angle0+2*da)), (float)(w*0.5));
        glVertex3f((float)(r1*cos(angle0)+3.0*da), (float)(r1*sin(angle0)+3.0*da), (float)w*0.5);
    }
    glEnd();
    glNormal3f(0.0,0.0,-1.0);
    glBegin(GL_QUAD_STRIP);
      for(i = 0; i <= teeth; i++)
      {
          angle0 = (float)(i*2.0*PI/teeth);
          if( i >2 && i < teeth-2)
          {
              glVertex3f((float)(r1*cos(angle0)), (float)(r1*sin(angle0)), (float)(-w*0.5));
              glVertex3f((float)(r0*cos(angle0)), (float)(r0*sin(angle0)), (float)(-w*0.5));
              glVertex3f((float)(r1*cos(angle0)+3.0*da), (float)(r1*sin(angle0)+3.0*da), (float)-w*0.5);
              glVertex3f((float)(r0*cos(angle0)), (float)(r0*sin(angle0)), (float)(-w*0.5));
          }
          else
          {
              glVertex3f((float)(r1*cos(angle0)), (float)(r1*sin(angle0)), (float)(-w*0.5));
              glVertex3f((float)(r0*1.2), (float)(r0*sin(angle0)), (float)-w*0.5);
              glVertex3f((float)(r1*cos(angle0)+3.0*da), (float)(r1*sin(angle0)+3.0*da), (float)-w*0.5);
              glVertex3f((float)(r0*1.2), (float)(r0*sin(angle0)), (float)-w*0.5);
          }
      }
    glEnd();
    /**draw the back of the teeth*/
    glBegin(GL_QUADS);
      da = (float)(2.0*PI/teeth/4.0);
      for( i =0; i < teeth; i++)
      {
          angle0 = (float)(i*2.0*PI/teeth);
          glVertex3f((float)(r1*cos(angle0)+3.0*da), (float)(r1*sin(angle0)+3.0*da), (float)-w*0.5);
          glVertex3f((float)(r2*cos(angle0+2*da)), (float)(r2*sin(angle0+2*da)), (float)(-w*0.5));
          glVertex3f((float)(r2*cos(angle0+da)), (float)(r2*sin(angle0+da)), (float)(-w*0.5));
          glVertex3f((float)(r1*cos(angle0)), (float)(r1*sin(angle0)), (float)(-w*0.5));
      }
    glEnd();

    /*draw the out ward of the teeth*/
    glBegin(GL_QUAD_STRIP);
      for( i = 0; i < teeth; i++)
      {
          angle0 = (float)(i*2.0*PI/teeth);
          glVertex3f((float)(r1*cos(angle0)), (float)(r1*sin(angle0)),(float)(w*0.5));
          glVertex3f((float)(r1*cos(angle0)), (float)(r1*sin(angle0)),(float)(-w*0.5));
          u = (float)(r2*cos(angle0+da) - r1*cos(angle0));
          v = (float)(r2*sin(angle0+da) - r1*sin(angle0));
          len = (float)sqrt(u*u + v*v);
          u /= len;
          v /= len;
          glNormal3f(v,-u, 0.0);
          glVertex3f((float)(r2*cos(angle0+da)),(float)(r2*sin(angle0+da)), (float(w*0.5)));
          glVertex3f((float)(r2*cos(angle0+da)),(float)(r2*sin(angle0+da)), (float(-w*0.5)));
          glVertex3f((float)(r2*cos(angle0+2*da)), (float)(r2*sin(angle0+2*da)), (float)(w*0.5));
          glVertex3f((float)(r2*cos(angle0+2*da)), (float)(r2*sin(angle0+2*da)), (float)(-w*0.5));
          u = (float)(r1*cos(angle0+3.0*da) - r2*cos(angle0+2.0*da));
          v = (float)(r1*sin(angle0+3.0*da) - r2*sin(angle0+2.0*da));
          glNormal3f(v,-u,0.0);
          glVertex3f((float)(r1*cos(angle0)+3.0*da), (float)(r1*sin(angle0)+3.0*da), (float)w*0.5);
          glVertex3f((float)(r1*cos(angle0)+3.0*da), (float)(r1*sin(angle0)+3.0*da), (float)-w*0.5);
          glVertex3f((float)cos(angle0),(float)sin(angle0),0.0);
      }
      glVertex3f((float)r1*cos(0), (float)(r1*sin(0)), (float)(w*0.5));
      glVertex3f((float)r1*cos(0), (float)(r1*sin(0)), (float)(-w*0.5));
    glEnd();

    /*draw the inside of the radius cylinder*/

    glBegin(GL_QUAD_STRIP);
      for( i = 0; i <= teeth; i++ )
      {

      }
    glEnd();
}


/*****The all timer,mouse and keyboard events******/
void GLWidget::timerEvent(QTimerEvent *)
{
    zRot += xSpeed;
    yRot += ySpeed;
    angle += 360/teethnum;
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    lasPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    GLfloat dx = GLfloat( e->pos().x() - lasPos.x() ) / width();
    GLfloat dy = GLfloat( e->pos().y() - lasPos.y() ) / height();

    if( e->buttons() & Qt::LeftButton )
    {
        zRot += 90 * dx;
        yRot += 90 * dy;
    }
    else if ( e->buttons() & Qt::RightButton )
    {
        xSpeed = 18 * dx;
        ySpeed = 18 * dy;
    }
    updateGL();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    Go_init();
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    e->delta()>0 ? scale += scale*0.1 : scale -= scale*0.1;
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch( e->key() )
    {
    case Qt::Key_Left:
        if( e->modifiers() & Qt::ControlModifier )
            xSpeed = -10;
        else
            zRot -= 18;
        break;
    case Qt::Key_Right:
        if( e->modifiers() & Qt::ControlModifier )
            xSpeed = +10;
        else
            zRot += 18;
        break;
    case Qt::Key_Up:
        if( e->modifiers() & Qt::ControlModifier )
            ySpeed = -10;
        else
            yRot -= 18;
        break;
    case Qt::Key_Down:
        if( e->modifiers() & Qt::ControlModifier )
            ySpeed = +10;
        else
            yRot += 18;
        break;
    case Qt::Key_PageUp:
        zoom -= zoom * 0.1;
        break;
    case Qt::Key_PageDown:
        zoom += zoom * 0.1;
        break;
    case Qt::Key_Plus:
        scale += 0.1;
        break;
    case Qt::Key_Minus:
        scale -= 0.1;
        break;
    case Qt::Key_S:
        killTimer(t);
        break;
    case Qt::Key_R:
        t = startTimer(50);
        break;
    case Qt::Key_Home: //回到初始状态
        if( e->modifiers() & Qt::ControlModifier )
            Go_init();
        break;
    case Qt::Key_X: //观察X面
        if( e->modifiers() & Qt::ControlModifier )
            Go_init();
        break;
    case Qt::Key_Y:
        if( e->modifiers() & Qt::ControlModifier )
        {
            Go_init();
            zRot = -90;
        }
        break;
    case Qt::Key_Z:
        if( e->modifiers() & Qt::ControlModifier )
        {
            Go_init();
            yRot = 90;
        }
        break;
    case Qt::Key_Escape:
        close();
        break;
    }
    updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{

}

void GLWidget::Go_init()
{
    xRot = yRot = zRot = 0.0;
    xSpeed = ySpeed = 0.0;
}

/*****The all timer,mouse and keyboard events******/
