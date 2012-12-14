#include "glwidget.h"

static GLfloat colors[12][3]=				// 彩虹颜色
{
    {1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
    {0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
    {0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};


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
    MAX_PARTICLES = 1000;
    PP = new Particles[MAX_PARTICLES];
}

GLWidget::~GLWidget()
{
    makeCurrent();
    delete [] PP;
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
    //InitPar();
    t = startTimer(50);
}

void GLWidget::InitPar()
{
    glDisable(GL_DEPTH_TEST);
    for ( int i = 0; i < MAX_PARTICLES; i++ )
    {
        PP[i].isActive = true;
        PP[i].life = 1.0f;
        PP[i].fade = float(rand() % 100)/1000.0+0.003f;
        PP[i].r=colors[i*(12/MAX_PARTICLES)][0];		// 粒子的红色颜色
        PP[i].g=colors[i*(12/MAX_PARTICLES)][1];		// 粒子的绿色颜色
        PP[i].b=colors[i*(12/MAX_PARTICLES)][2];		// 粒子的蓝色颜色
        PP[i].xi=float((rand()%50)-26.0f)*10.0f;		// 随机生成X轴方向速度
        PP[i].yi=float((rand()%50)-25.0f)*10.0f;		// 随机生成Y轴方向速度
        PP[i].zi=float((rand()%50)-25.0f)*10.0f;		// 随机生成Z轴方向速度
        PP[i].xg= 0.0f;						// 设置X轴方向加速度为0
        PP[i].yg=-0.8f;						//  设置Y轴方向加速度为-0.8
        PP[i].zg= 0.0f;						//  设置Z轴方向加速度为0
    }
}

void GLWidget::drawPar()
{
    glLoadIdentity();
    setRotate();
    for( int i = 0; i < MAX_PARTICLES; i++ )
    {
        if( PP[i].isActive)
        {
            float x = PP[i].x;
            float y = PP[i].y;
            float z = PP[i].z + zoom;
            glColor4f(PP[i].r,PP[i].g,PP[i].b,PP[i].life);
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2d(1,1); glVertex3f(x+0.5f,y+0.5f,z);
            glTexCoord2d(0,1); glVertex3f(x-0.5f,y+0.5f,z);
            glTexCoord2d(1,0); glVertex3f(x+0.5f,y-0.5f,z);
            glTexCoord2d(0,0); glVertex3f(x-0.5f,y-0.5f,z);
            glEnd();
            PP[i].x+=PP[i].xi/(slowdown*1000);	// 更新X坐标的位置
            PP[i].y+=PP[i].yi/(slowdown*1000);	// 更新Y坐标的位置
            PP[i].z+=PP[i].zi/(slowdown*1000);	// 更新Z坐标的位置
            PP[i].xi+=PP[i].xg;			// 更新X轴方向速度大小
            PP[i].yi+=PP[i].yg;			// 更新Y轴方向速度大小
            PP[i].zi+=PP[i].zg;			// 更新Z轴方向速度大小

            PP[i].life-=PP[i].fade;

            if( PP[i].life < 0.0f)
            {
                PP[i].life=1.0f;				// 产生一个新的粒子
                PP[i].fade=float(rand()%100)/1000.0f+0.003f;	// 随机生成衰减速率
                PP[i].x=0.0f;					// 新粒子出现在屏幕的中央
                PP[i].y=0.0f;
                PP[i].z=0.0f;
                PP[i].xi=xSpeed+float((rand()%60)-32.0f);	// 随机生成粒子速度
                PP[i].yi=ySpeed+float((rand()%60)-30.0f);
                PP[i].zi=float((rand()%60)-30.0f);
                PP[i].r=colors[col][0];			// 设置粒子颜色
                PP[i].g=colors[col][1];
                PP[i].b=colors[col][2];
            }
        }
    }

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
