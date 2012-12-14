#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setWindowTitle("openGL test_03");
    resize(400,400);
    makeCurrent();
    xRot=yRot=zRot=0.0;
    xSpeed = ySpeed = 0.0;
    scale = 1.0;
    zoom = -10.0;
    glFogCoordfEXT = NULL;
}

GLWidget::~GLWidget()
{
    makeCurrent();
}

bool GLWidget::getExtensions()
{
    char Extension_Name[] = "EXT_fog_coord";
#ifdef Q_WS_X11
    char* glextString = ( char* ) malloc(strlen((char*)glGetString(GL_EXTENSIONS))+1);
    strcpy(glextString,(char*)glGetString(GL_EXTENSIONS));
    if( !strstr(glextString,Extension_Name))
        return false;
    free(glextString);
    const GLubyte extension[]  = "glFogCoordfEXT";
    glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC)glXGetProcAddress(extension);
#else
    char* glextString = ( char* ) malloc(strlen((char*)glGetString(GL_EXTENSIONS))+1);
    strcpy(glextString,(char*)glGetString(GL_EXTENSIONS));
    if( !strstr(glextString,Extension_Name))
        return false;
    free(glextString);
    glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC)wglGetProcAddress("glFogCoordfEXT");
#endif
    return true;
}

void GLWidget::initializeGL()
{
    if(!getExtensions())
    {
        fprintf(stderr,"Error: %s.\n","Open OpenGL Extensions Error!!!");
        exit(-1);
    }
    qglClearColor(Qt::gray);
    glClearDepth(1.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    setLight();
    setFog();



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
    Draw();
    glFlush();
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

void GLWidget::setFog()
{
    GLfloat fogColor[4] = {0.6f,0.3f,0.0f,1.0f};
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE,GL_EXP);
    glFogfv(GL_FOG_COLOR,fogColor);
    glFogf(GL_FOG_DENSITY,0.5f);
    glFogf(GL_FOG_START,1.0f);
    glFogf(GL_FOG_END,5.0f);
    glHint(GL_FOG_HINT,GL_NICEST);
    glFogi(GL_FOG_COORDINATE_SOURCE_EXT,GL_FOG_COORDINATE_EXT);
}

void GLWidget::Draw()
{
    setRotate();
    glColor3f(1.0f,1.0f,1.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);

    glColor3f(0.0,0.0,1.0);
    glutSolidSphere(0.5,64,64);

    VolumeFog();
    //glutWireSphere(0.25,128,128);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_BLEND);
}

void GLWidget::VolumeFog()
{
    glLoadIdentity();
    setRotate();
    glEnable(GL_FOG);
    glBegin(GL_QUADS);							//后墙
            glFogCoordfEXT(0.0f); glVertex3f(-2.5f,-2.5f,-6.0f);
            glFogCoordfEXT(0.0f); glVertex3f( 2.5f,-2.5f,-6.0f);
            glFogCoordfEXT(0.0f); glVertex3f( 2.5f, 2.5f,-6.0f);
            glFogCoordfEXT(0.0f); glVertex3f(-2.5f, 2.5f,-6.0f);
        glEnd();

        glBegin(GL_QUADS);							// 地面
            glFogCoordfEXT(0.0f); glVertex3f(-2.5f,-2.5f,-6.0f);
            glFogCoordfEXT(0.0f); glVertex3f( 2.5f,-2.5f,-6.0f);
            glFogCoordfEXT(1.0f); glVertex3f( 2.5f,-2.5f, 6.0f);
            glFogCoordfEXT(1.0f); glVertex3f(-2.5f,-2.5f, 6.0f);
        glEnd();

        glBegin(GL_QUADS);							// 天花板
            glFogCoordfEXT(0.0f); glVertex3f(-2.5f, 2.5f,-6.0f);
            glFogCoordfEXT(0.0f); glVertex3f( 2.5f, 2.5f,-6.0f);
            glFogCoordfEXT(1.0f); glVertex3f( 2.5f, 2.5f, 6.0f);
            glFogCoordfEXT(1.0f); glVertex3f(-2.5f, 2.5f, 6.0f);
        glEnd();

        glBegin(GL_QUADS);							// 右墙
            glFogCoordfEXT(1.0f); glVertex3f( 2.5f,-2.5f, 6.0f);
            glFogCoordfEXT(1.0f); glVertex3f( 2.5f, 2.5f, 6.0f);
            glFogCoordfEXT(0.0f); glVertex3f( 2.5f, 2.5f,-6.0f);
            glFogCoordfEXT(0.0f); glVertex3f( 2.5f,-2.5f,-6.0f);
        glEnd();

        glBegin(GL_QUADS);							// 左墙
            glFogCoordfEXT(1.0f); glVertex3f(-2.5f,-2.5f, 6.0f);
            glFogCoordfEXT(1.0f); glVertex3f(-2.5f, 2.5f, 6.0f);
            glFogCoordfEXT(0.0f); glVertex3f(-2.5f, 2.5f,-6.0f);
            glFogCoordfEXT(0.0f); glVertex3f(-2.5f,-2.5f,-6.0f);
        glEnd();

        glBegin(GL_QUADS);							//前墙
            glFogCoordfEXT(1.0f); glVertex3f(-2.5f,-2.5f, 6.0f);
            glFogCoordfEXT(1.0f); glVertex3f( 2.5f,-2.5f, 6.0f);
            glFogCoordfEXT(1.0f); glVertex3f( 2.5f, 2.5f, 6.0f);
            glFogCoordfEXT(1.0f); glVertex3f(-2.5f, 2.5f, 6.0f);
        glEnd();
        glDisable(GL_FOG);

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
