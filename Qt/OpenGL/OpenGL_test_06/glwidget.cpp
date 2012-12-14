#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setWindowTitle("openGL test_06");
    resize(400,400);
    makeCurrent();
    xRot=yRot=zRot=0.0;
    xSpeed = ySpeed = 0.0;
    scale = 1.0;
    zoom = -10.0;
    MAX_PARTICLES = 1000;
    Cube_Width = Cube_Length = Cube_Height = 1.0f;
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
    setLight();

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
    gluPerspective( 45.0, x, 0.01,100.0);
    glMatrixMode(GL_MODELVIEW);

}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    Draw_Arrows();
    DrawCube();
    Draw();
    //drawPar();
//    glFlush();
//    swapBuffers();

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

    setFont(QFont("Times",18));
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

void GLWidget::DrawCube()
{
    glLoadIdentity();
    //glTranslatef(0.0f,0.0f,0.0f);
    setRotate();
    glColor3f(0.0,0.0,1.0);
    glLineWidth(3);
    glPushMatrix();
    glEnable(GL_LINE_SMOOTH);
//    glBegin(GL_LINE_LOOP);      //顶平面
//    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0);
//    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0);
//    glVertex3f( Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0);
//    glVertex3f( Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0);
//    glEnd();
    glBegin(GL_LINE_LOOP);      //底平面
    glVertex3f(-Cube_Width/2.0,-Cube_Height/2.0,-Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0,-Cube_Height/2.0, Cube_Length/2.0);
    glVertex3f( Cube_Width/2.0,-Cube_Height/2.0, Cube_Length/2.0);
    glVertex3f( Cube_Width/2.0,-Cube_Height/2.0,-Cube_Length/2.0);
    glEnd();
    glBegin(GL_LINE_LOOP);      //右平面
    glVertex3f( Cube_Width/2.0,-Cube_Height/2.0,-Cube_Length/2.0);
    glVertex3f( Cube_Width/2.0,-Cube_Height/2.0, Cube_Length/2.0);
    glVertex3f( Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0);
    glVertex3f( Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0);
    glEnd();

//    glBegin(GL_LINE_LOOP);      //左平面
//    glVertex3f(-Cube_Width/2.0,-Cube_Height/2.0,-Cube_Length/2.0);
//    glVertex3f(-Cube_Width/2.0,-Cube_Height/2.0, Cube_Length/2.0);
//    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0);
//    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0);
//    glEnd();
    glBegin(GL_LINES);           //补全其他线
    glVertex3f( Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0,-Cube_Height/2.0, Cube_Length/2.0);
    glEnd();

    glLineWidth(4);
    glColor3f(0.0,1.0,0.0);
    //绘制立方体上坐标轴
    glBegin(GL_LINES);  //绘制X坐标轴
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0);
    glVertex3f( Cube_Width/2.0+0.2, Cube_Height/2.0,-Cube_Length/2.0);
    glEnd();
    glColor3f(1.0,0.0,0.0);
    renderText(Cube_Width/2.0+0.22, Cube_Height/2.0,-Cube_Length/2.0,QChar('x'));

    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);  //绘制Y坐标轴
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0+0.2);
    glEnd();
    glColor3f(1.0,0.0,0.0);
    renderText(-Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0+0.2,QChar('y'));

    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);  //绘制Z坐标轴
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0,-Cube_Height/2.0-0.2,-Cube_Length/2.0);
    glEnd();
    glColor3f(1.0,0.0,0.0);
    renderText(-Cube_Width/2.0,-Cube_Height/2.0-0.22,-Cube_Length/2.0,QChar('z'));

    //绘制坐标轴上的刻度
    glLineWidth(0.2);
    glColor3f(0.0,0.0,1.0);
    int Loop = 0;

    //绘制X坐标刻度
    Loop = int(Cube_Width /0.1);
    for( int i = 0; i <= Loop; i++ )
    {
        glBegin(GL_LINES);
        glVertex3f(-Cube_Width/2.0+(float)i*0.1, Cube_Height/2.0,-Cube_Length/2.0);
        glVertex3f(-Cube_Width/2.0+(float)i*0.1, Cube_Height/2.0+0.01,-Cube_Length/2.0-0.01);
        glEnd();
        renderText(-Cube_Width/2.0+(float)i*0.1, Cube_Height/2.0+0.015,-Cube_Length/2.0-0.015,QString::number(i*1000));

//        //绘制网格线
//        glBegin(GL_LINE_LOOP);
//        glVertex3f(-Cube_Width/2.0+(float)i*0.1, Cube_Height/2.0,-Cube_Length/2.0);
//        glVertex3f(-Cube_Width/2.0+(float)i*0.1,-Cube_Height/2.0,-Cube_Length/2.0);
//        glVertex3f(-Cube_Width/2.0+(float)i*0.1,-Cube_Height/2.0, Cube_Length/2.0);
//        glVertex3f(-Cube_Width/2.0+(float)i*0.1, Cube_Height/2.0, Cube_Length/2.0);
//        glEnd();

    }

    //绘制Y坐标刻度
    Loop = int(Cube_Length /0.1);
    for( int i = 1; i <= Loop; i++ )
    {
        glBegin(GL_LINES);
        glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0+float(i)*0.1);
        glVertex3f(-Cube_Width/2.0-0.01, Cube_Height/2.0+0.01,-Cube_Length/2.0+float(i)*0.1);
        glEnd();
        renderText(-Cube_Width/2.0-0.015, Cube_Height/2.0+0.015,-Cube_Length/2.0+float(i)*0.1,QString::number(i*1000));

//        //绘制网格线
//        glBegin(GL_LINE_LOOP);
//        glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0+float(i)*0.1);
//        glVertex3f(-Cube_Width/2.0,-Cube_Height/2.0,-Cube_Length/2.0+float(i)*0.1);
//        glVertex3f( Cube_Width/2.0,-Cube_Height/2.0,-Cube_Length/2.0+float(i)*0.1);
//        glVertex3f( Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0+float(i)*0.1);
//        glEnd();
    }
    //绘制Z坐标刻度
    Loop = int(Cube_Height /0.1);
    for( int i = 1; i <= Loop; i++ )
    {
        glBegin(GL_LINES);
        glVertex3f(-Cube_Width/2.0, Cube_Height/2.0-float(i)*0.1,-Cube_Length/2.0);
        glVertex3f(-Cube_Width/2.0-0.01, Cube_Height/2.0-float(i)*0.1,-Cube_Length/2.0-0.01);
        glEnd();
        renderText(-Cube_Width/2.0-0.015, Cube_Height/2.0-float(i)*0.1,-Cube_Length/2.0-0.015,QString::number(i*1000));

//        //绘制网格线
//        glBegin(GL_LINE_LOOP);
//        glVertex3f(-Cube_Width/2.0, Cube_Height/2.0-float(i)*0.1,-Cube_Length/2.0);
//        glVertex3f(-Cube_Width/2.0, Cube_Height/2.0-float(i)*0.1, Cube_Length/2.0);
//        glVertex3f( Cube_Width/2.0, Cube_Height/2.0-float(i)*0.1, Cube_Length/2.0);
//        glVertex3f( Cube_Width/2.0, Cube_Height/2.0-float(i)*0.1,-Cube_Length/2.0);
//        glEnd();
    }


    glDisable(GL_LINE_SMOOTH);
    glPopMatrix();
}

void GLWidget::setLight()
{
    GLfloat mat_ambient [] = {1.0,1.0,1.0,0.0};
    GLfloat mat_diffuse [] = {1.0,1.0,1.0,0.0};
    GLfloat mat_specular [] = {1.0,1.0,1.0,1.0};
    GLfloat mat_shininess []={50.0};
    GLfloat light_ambient [] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse [] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position [] = {- 4.0,4.0,4.0,0.0};

    GLfloat white_light [] = {1.0,1.0,1.0,1.0};
    GLfloat lmodel_ambient [] = { 0.8,0.8,0.8,1.0};

    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS, mat_shininess );
    glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse );
    glMaterialfv(GL_FRONT,GL_AMBIENT, mat_ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient );
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0,GL_POSITION, light_position );
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light );
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient );
    //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}


void GLWidget::Draw()
{
    setRotate();
    glColor3f(1.0f,1.0f,1.0f);
    glEnable(GL_LIGHTING);

    glColor3f(0.0,0.0,1.0);
    glutSolidSphere(0.5,64,64);


    //glutWireSphere(0.25,128,128);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
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
