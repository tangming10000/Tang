#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setWindowTitle("openGL test_01");
    resize(400,400);
    makeCurrent();
    xRot=yRot=zRot=0.0;
    xSpeed = ySpeed = 0.0;
    scale = 1.0;
    zoom = -10.0;
}

GLWidget::~GLWidget()
{
    makeCurrent();
}

void GLWidget::initializeGL()
{
    qglClearColor(Qt::gray);
    glClearDepth(1.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    t = startTimer(50);
    DisplayList();
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

    //FillTorus(0.12,8,1.0,25);
    setRotate();
    Draw();
    //glCallList(theTorus);
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
    setRotate();
    GLfloat length = 2.0;
    int i,j,k;
    glPushMatrix();
    for( i = -1; i <= 1; i++)
        for( j = -1; j <= 1; j++)
            for( k = -1; k <= 1; k++)
            {
                glPushMatrix();
                glTranslatef(length*i*1.05,length*j*1.05,length*k*1.05);
                cube(length);
                glPopMatrix();
            }
    glPopMatrix();
    swapBuffers();
    /////////////////////////////////
    /*
    glPushMatrix();
    glBegin( GL_QUADS );
       glColor3f( 0.0, 1.0, 0.0 );
       glVertex3f(  1.0,  1.0, -1.0 );
       glVertex3f( -1.0,  1.0, -1.0 );
       glVertex3f( -1.0,  1.0,  1.0 );
       glVertex3f(  1.0,  1.0,  1.0 );glColor3f( 1.0, 0.5, 0.0 );
       glVertex3f(  1.0, -1.0,  1.0 );
       glVertex3f( -1.0, -1.0,  1.0 );
       glVertex3f( -1.0, -1.0, -1.0 );
       glVertex3f(  1.0, -1.0, -1.0 );glColor3f( 1.0, 0.0, 0.0 );
       glVertex3f(  1.0,  1.0,  1.0 );
       glVertex3f( -1.0,  1.0,  1.0 );
       glVertex3f( -1.0, -1.0,  1.0 );
       glVertex3f(  1.0, -1.0,  1.0 );glColor3f( 1.0, 1.0, 0.0 );
       glVertex3f(  1.0, -1.0, -1.0 );
       glVertex3f( -1.0, -1.0, -1.0 );
       glVertex3f( -1.0,  1.0, -1.0 );
       glVertex3f(  1.0,  1.0, -1.0 );glColor3f( 0.0, 0.0, 1.0 );
       glVertex3f( -1.0,  1.0,  1.0 );
       glVertex3f( -1.0,  1.0, -1.0 );
       glVertex3f( -1.0, -1.0, -1.0 );
       glVertex3f( -1.0, -1.0,  1.0 );glColor3f( 1.0, 0.0, 1.0 );
       glVertex3f(  1.0,  1.0, -1.0 );
       glVertex3f(  1.0,  1.0,  1.0 );
       glVertex3f(  1.0, -1.0,  1.0 );
       glVertex3f(  1.0, -1.0, -1.0 );
       glEnd();
       glPopMatrix();
       */
}

void GLWidget::cube(GLfloat length)
{
    static int COLORNUM = 26;
    int indcolor = 0, rgbcolor = 0;
    static GLfloat colors[][3] =
    { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.5f }, { 0.0f, 0.0f, 1.0f },
      { 0.0f, 0.5f, 0.0f }, { 0.0f, 0.5f, 0.5f }, { 0.0f, 0.5f, 1.0f },
      { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.5f }, { 0.0f, 1.0f, 1.0f },//
      { 0.5f, 0.0f, 0.0f }, { 0.5f, 0.0f, 0.5f }, { 0.5f, 0.0f, 1.0f },
      { 0.5f, 0.5f, 0.0f }, { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 1.0f },
      { 0.5f, 1.0f, 0.0f }, { 0.5f, 1.0f, 0.5f }, { 0.5f, 1.0f, 1.0f },//
      { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.5f }, { 1.0f, 0.0f, 1.0f },
      { 1.0f, 0.5f, 0.0f }, { 1.0f, 0.5f, 0.5f }, { 1.0f, 0.5f, 1.0f },
      { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.5f }, { 1.0f, 1.0f, 1.0f },
    };
    glBegin(GL_QUAD_STRIP);
        /*front face*/
        glColor3fv(colors[rgbcolor]);
        rgbcolor++; rgbcolor %= COLORNUM;
        glVertex3f(-length/2, length/2, length/2);//1
        glColor3fv(colors[rgbcolor]);
        rgbcolor++; rgbcolor %= COLORNUM;
        glVertex3f(-length/2,-length/2, length/2);//2
        glColor3fv(colors[rgbcolor]);
        rgbcolor++; rgbcolor %= COLORNUM;
        glVertex3f( length/2, length/2, length/2);//3
        glColor3fv(colors[rgbcolor]);
        rgbcolor++; rgbcolor %= COLORNUM;
        glVertex3f( length/2,-length/2, length/2);//4
        /*right face*/
        glColor3fv(colors[rgbcolor]);
        rgbcolor++; rgbcolor %= COLORNUM;
        glVertex3f( length/2, length/2,-length/2);//5
        glColor3fv(colors[rgbcolor]);
        rgbcolor++; rgbcolor %= COLORNUM;
        glVertex3f( length/2,-length/2,-length/2);//6
        /*back face*/
        glColor3fv(colors[rgbcolor]);
        rgbcolor++; rgbcolor %= COLORNUM;
        glVertex3f(-length/2, length/2,-length/2);//7
        glColor3fv(colors[rgbcolor]);
        rgbcolor++; rgbcolor %= COLORNUM;
        glVertex3f(-length/2,-length/2,-length/2);//8
        /*left face*/
        glColor3fv(colors[rgbcolor]);
        rgbcolor++; rgbcolor %= COLORNUM;
        glVertex3f(-length/2, length/2, length/2);//9
        glColor3fv(colors[rgbcolor]);
        rgbcolor++; rgbcolor %= COLORNUM;
        glVertex3f(-length/2,-length/2, length/2);//10
    glEnd();
    /*uppper face*/
    glBegin(GL_QUADS);
        glColor3fv(colors[rgbcolor]);
        rgbcolor ++; rgbcolor %= COLORNUM;
        glVertex3f(-length/2, length/2, length/2);//1
        glColor3fv(colors[rgbcolor]);
        rgbcolor ++; rgbcolor %= COLORNUM;
        glVertex3f( length/2, length/2, length/2);//2
        glColor3fv(colors[rgbcolor]);
        rgbcolor ++; rgbcolor %= COLORNUM;
        glVertex3f( length/2, length/2,-length/2);//3
        glColor3fv(colors[rgbcolor]);
        rgbcolor ++; rgbcolor %= COLORNUM;
        glVertex3f(-length/2, length/2,-length/2);//4
    glEnd();
    /*lower face*/
    glBegin(GL_QUADS);
        glColor3fv(colors[rgbcolor]);
        rgbcolor ++; rgbcolor %= COLORNUM;
        glVertex3f(-length/2,-length/2, length/2);//1
        glColor3fv(colors[rgbcolor]);
        rgbcolor ++; rgbcolor %= COLORNUM;
        glVertex3f( length/2,-length/2, length/2);//2
        glColor3fv(colors[rgbcolor]);
        rgbcolor ++; rgbcolor %= COLORNUM;
        glVertex3f( length/2,-length/2,-length/2);//3
        glColor3fv(colors[rgbcolor]);
        rgbcolor ++; rgbcolor %= COLORNUM;
        glVertex3f(-length/2,-length/2,-length/2);//4
    glEnd();

}

void GLWidget::FillTorus(float rc, int numc, float rt, int numt)
{
    int i,j,k;
    double s,t;
    double x,y,z;
    double pi,twopi;
    pi = 3.141592653589;
    twopi = 2*pi;
    for( i = 0; i < numc; i++)
    {
        glBegin(GL_QUAD_STRIP);
            for( j = 0; j <= numt; j++)
            {
                for( k = 1; k >= 0; k--)
                {
                    s = (i+k)%numc + 0.5;
                    t = j%numt;
                    x = cos(t*twopi/numt)*cos(s*twopi/numc);
                    y = sin(t*twopi/numt)*cos(s*twopi/numc);
                    z = sin(s*twopi/numc);
                    glNormal3f(x,y,z);
                    x = (rt + rc*cos(s*twopi/numc))*cos(t*twopi/numt);
                    y = (rt + rc*cos(s*twopi/numc))*sin(t*twopi/numt);
                    z = rc*sin(t*twopi/numt);
                    glVertex3f(x,y,z);
                }
            }
        glEnd();
    }
}

void GLWidget::DisplayList()
{
    theTorus = glGenLists(1);
    glNewList(theTorus,GL_COMPILE);
        FillTorus(0.12,8,1.0,25);
    glEndList();
}

/*****The all timer,mouse and keyboard events******/
void GLWidget::timerEvent(QTimerEvent *)
{
    zRot += xSpeed;
    yRot += ySpeed;
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
