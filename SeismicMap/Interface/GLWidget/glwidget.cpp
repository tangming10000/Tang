#include "glwidget.h"

GLWidget::GLWidget(MainWindow *parent)
{
    setWindowTitle("openGL test_06");
    //resize(400,400);
    makeCurrent();
    xRot=yRot=zRot=0.0;
    xSpeed = ySpeed = 0.0;
    scale = 1.0;
    zoom = -10.0;
    Cube_Width = Cube_Length = Cube_Height = 1.0f;
    XMin = YMin = 0;
    ListNum.clear();
    Total_ListNum = 0;

}

GLWidget::~GLWidget()
{
    ListNum.clear();
    makeCurrent();
    dm = NULL;
}

void GLWidget::SetData(DataManager *DM)
{
    dm = DM;
    Cube_Width = dm->mapset->Max_XLine - dm->mapset->Min_XLine;
    Cube_Length= dm->mapset->Max_YLine - dm->mapset->Min_YLine;
    Cube_Height= dm->Reel->hns;
    dx = dm->seisset->dx;
    dt = dm->seisset->dy;
    XMin = dm->mapset->Min_XLine;
    YMin = dm->mapset->Min_YLine;

    Cube_Width *= 0.001f*dx;
    Cube_Length*= 0.001f*dx;
    Cube_Height*= 0.001f*dt;

    Draw_SeisColor();
    //Draw_Surface();
}

void GLWidget::initializeGL()
{

    qglClearColor(Qt::gray);
    glClearDepth(1.0);
    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
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
    glPushMatrix();
    Draw_Arrows();
    DrawCube();
    glPushMatrix();
    Draw();
    glPopMatrix();
    glPopMatrix();

    //swapBuffers();

}

void GLWidget::setRotate()
{
    glLoadIdentity();
    glTranslatef(0.0,0.0,zoom);
    glRotatef(yRot,0.0,0.0,1.0);
    glRotatef(xRot,1.0,0.0,0.0);
    glRotatef(zRot,0.0,1.0,0.0);

    glScalef(scale,scale,scale);
}

void GLWidget::Draw_Arrows()
{
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(-0.9f,-0.6f,-2.0f);
    glRotatef(yRot,0.0f,0.0f,1.0f);
    glRotatef(xRot,1.0f,0.0f,0.0f);
    glRotatef(zRot,0.0f,1.0f,0.0f);

    setFont(QFont("Times",18));
    glLineWidth(4);
    glEnable(GL_LINE_SMOOTH);

    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_LINES);
      glVertex3f(0.0f,0.0f,0.0f);
      glVertex3f(0.1f,0.0f,0.0f);
    glEnd();
    renderText(0.2f,0.0f,0.0f,QChar('x'));

    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_LINES);
      glVertex3f(0.0f,0.0f,0.0f);
      glVertex3f(0.0f,-0.1f,0.0f);
    glEnd();
    renderText(0.0f,-0.2f,0.0f,QChar('z'));

    glColor3f(0.0f,0.0f,1.0f);
    glBegin(GL_LINES);
      glVertex3f(0.0f,0.0f,0.0f);
      glVertex3f(0.0f,0.0f,0.1f);
    glEnd();
    renderText(0.0f,0.0f,0.2f,QChar('y'));
    glPopMatrix();

    glDisable(GL_LINE_SMOOTH);

 }

void GLWidget::DrawCube()
{
    glLoadIdentity();
    setRotate();
    glColor3f(0.0f,0.0f,1.0f);
    glLineWidth(3);
    glPushMatrix();
    glEnable(GL_LINE_SMOOTH);

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

    glBegin(GL_LINES);           //补全其他线
    glVertex3f( Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0,-Cube_Height/2.0, Cube_Length/2.0);
    glEnd();

    glLineWidth(4);
    glColor3f(0.0f,1.0f,0.0f);
    //绘制立方体上坐标轴
    glBegin(GL_LINES);  //绘制X坐标轴
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0);
    glVertex3f( Cube_Width/2.0+0.2, Cube_Height/2.0,-Cube_Length/2.0);
    glEnd();
    glColor3f(1.0f,0.0f,0.0f);
    renderText(Cube_Width/2.0+0.22, Cube_Height/2.0,-Cube_Length/2.0,QChar('x'));

    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_LINES);  //绘制Y坐标轴
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0+0.2);
    glEnd();
    glColor3f(1.0f,0.0f,0.0f);
    renderText(-Cube_Width/2.0, Cube_Height/2.0, Cube_Length/2.0+0.2,QChar('y'));

    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_LINES);  //绘制Z坐标轴
    glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0);
    glVertex3f(-Cube_Width/2.0,-Cube_Height/2.0-0.2,-Cube_Length/2.0);
    glEnd();
    glColor3f(1.0f,0.0f,0.0f);
    renderText(-Cube_Width/2.0,-Cube_Height/2.0-0.22,-Cube_Length/2.0,QChar('z'));

    //绘制坐标轴上的刻度
    glLineWidth(2);
    glColor3f(0.0f,0.0f,1.0f);
    int Loop = 0;

    //绘制X坐标刻度
    Loop = int(Cube_Width/0.1/dx);
    for( int i = 0; i <= Loop; i++ )
    {
        glBegin(GL_LINES);
        glVertex3f(-Cube_Width/2.0+(float)i*0.1*dx, Cube_Height/2.0,-Cube_Length/2.0);
        glVertex3f(-Cube_Width/2.0+(float)i*0.1*dx, Cube_Height/2.0+0.01,-Cube_Length/2.0-0.01);
        glEnd();
        renderText(-Cube_Width/2.0+(float)i*0.1*dx, Cube_Height/2.0+0.015,-Cube_Length/2.0-0.015,QString::number(XMin+i*100));

//        //绘制网格线
//        glBegin(GL_LINE_LOOP);
//        glVertex3f(-Cube_Width/2.0+(float)i*0.1, Cube_Height/2.0,-Cube_Length/2.0);
//        glVertex3f(-Cube_Width/2.0+(float)i*0.1,-Cube_Height/2.0,-Cube_Length/2.0);
//        glVertex3f(-Cube_Width/2.0+(float)i*0.1,-Cube_Height/2.0, Cube_Length/2.0);
//        glVertex3f(-Cube_Width/2.0+(float)i*0.1, Cube_Height/2.0, Cube_Length/2.0);
//        glEnd();

    }

    //绘制Y坐标刻度
    Loop = int(Cube_Length /0.1/dx);
    for( int i = 1; i <= Loop; i++ )
    {
        glBegin(GL_LINES);
        glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0+float(i)*0.1*dx);
        glVertex3f(-Cube_Width/2.0-0.01, Cube_Height/2.0+0.01,-Cube_Length/2.0+float(i)*0.1*dx);
        glEnd();
        renderText(-Cube_Width/2.0-0.015, Cube_Height/2.0+0.015,-Cube_Length/2.0+float(i)*0.1*dx,QString::number(YMin+i*100));

//        //绘制网格线
//        glBegin(GL_LINE_LOOP);
//        glVertex3f(-Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0+float(i)*0.1);
//        glVertex3f(-Cube_Width/2.0,-Cube_Height/2.0,-Cube_Length/2.0+float(i)*0.1);
//        glVertex3f( Cube_Width/2.0,-Cube_Height/2.0,-Cube_Length/2.0+float(i)*0.1);
//        glVertex3f( Cube_Width/2.0, Cube_Height/2.0,-Cube_Length/2.0+float(i)*0.1);
//        glEnd();
    }
    //绘制Z坐标刻度
    Loop = int(Cube_Height /0.1/dt);
    for( int i = 1; i <= Loop; i++ )
    {
        glBegin(GL_LINES);
        glVertex3f(-Cube_Width/2.0, Cube_Height/2.0-float(i)*0.1*dt,-Cube_Length/2.0);
        glVertex3f(-Cube_Width/2.0-0.01, Cube_Height/2.0-float(i)*0.1*dt,-Cube_Length/2.0-0.01);
        glEnd();
        renderText(-Cube_Width/2.0-0.015, Cube_Height/2.0-float(i)*0.1*dt,-Cube_Length/2.0-0.015,QString::number(i*100));

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
    //glEnable(GL_LIGHTING);

    //glutSolidSphere(0.5,64,64);

//    glPushMatrix();
//    glBegin(GL_QUADS);
//    glColor3f(1.0f,0.0f,0.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
//    glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f, 1.0f,-1.0f);
//    glColor3f(0.0f,0.0f,1.0f);glVertex3f( 1.0f,-1.0f,-1.0f);
//    glColor3f(1.0f,1.0f,0.0f);glVertex3f( 1.0f,-1.0f, 1.0f);
//    glEnd();
//    glPopMatrix();

    if( ListNum.size() > 0 )
    {
        for( int i  = 0; i < ListNum.size(); i++ )
        {
            glPushMatrix();
            glCallList(ListNum.at(i));
            glPopMatrix();
        }
    }
    //glutWireSphere(0.25,128,128);
    //glDisable(GL_LIGHTING);
    //glDisable(GL_BLEND);
}

void GLWidget::getColor(float value)
{
//    float tt = (value+1.0f)/2.0f;
//    color[0] = value;
//    color[1] = value;
//    color[2] = value;

    if( value > 0)
    {
        color[0] = value;
        color[1] = 0.2f;
        color[2] = 0.3f;
    }
    else
    {
        color[0] = 0.2f;
        color[1] =-value;
        color[2] = 0.3f;
    }
}

void GLWidget::Draw_TraceColor(ShowTrace *t1, ShowTrace *t2)
{
    int N = dm->Reel->hns;
    GLfloat x1,y1,x2,y2,z11,z12,z21,z22;
    x1 = (t1->X - dm->mapset->Min_XLine)*0.001f*dx-Cube_Width/2.0;
    y1 = (t1->Y - dm->mapset->Min_YLine)*0.001f*dx-Cube_Length/2.0;
    x2 = (t2->X - dm->mapset->Min_XLine)*0.001f*dx-Cube_Width/2.0;
    y2 = (t2->Y - dm->mapset->Min_YLine)*0.001f*dx-Cube_Length/2.0;
    z11 = t1->data[0];
    z21 = t2->data[0];
    glPushMatrix();
    for( int k = 1; k < N; k++ )
    {
        z12 = t1->data[k];
        z22 = t2->data[k];

        glBegin(GL_QUADS);
        getColor(z11); glColor3fv(color);
        glVertex3f(x1,Cube_Height/2.0f-(k-1)*0.001f*dt,y1);
        getColor(z21); glColor3fv(color);
        glVertex3f(x2,Cube_Height/2.0f-(k-1)*0.001f*dt,y2);
        getColor(z22); glColor3fv(color);
        glVertex3f(x2,Cube_Height/2.0f-(k)*0.001f*dt,y2);
        getColor(z12); glColor3fv(color);
        glVertex3f(x1,Cube_Height/2.0f-(k)*0.001f*dt,y1);
        glEnd();
        z11 = z12;
        z21 = z22;
    }
    glPopMatrix();
}

//绘制地震剖面图像
void GLWidget::Draw_SeisColor()
{  
    ShowTrace *t1,*t2;
    int size = dm->Total_Slices.size();
    qDebug()<<size;
    GLuint traceList = 0;
    Total_ListNum = 1;    //计数

    for( int i = 0; i < size; i++ )
    {
        //t1 = dm->Total_Slices[i]->slice[0];
        t1 = dm->Total_Slices.at(i)->slice.at(0);
        for( int j = 1; j < dm->Total_Slices[i]->slice.size(); j++ )
        {
            t2 = dm->Total_Slices[i]->slice[j];
            traceList = glGenLists(Total_ListNum);
            glNewList(traceList,GL_COMPILE);
            Draw_TraceColor(t1,t2);
            glEndList();
            ListNum.push_back(traceList);
            t1 = t2;
            Total_ListNum ++;
        }
    }
    t1 = NULL;
    t2 = NULL;
}

void GLWidget::Draw_Surface()
{
    if ( dm->surface.size() > 0)
    {
        GLuint NN = glGenLists(Total_ListNum);
        Total_ListNum++;
        glNewList(NN,GL_COMPILE);

        glLoadIdentity();
        GLfloat x1,y1,z1,x2,y2,z2;
        int Min_X,Min_Y;
        Min_X = dm->mapset->Min_XLine;
        Min_Y = dm->mapset->Min_YLine;
        //glPointSize(5);
        glLineWidth(4);

        x1 = (dm->surface[0].x - Min_X)*0.001f*dx - Cube_Width/2.0;
        y1 = (dm->surface[0].y - Min_Y)*0.001f*dx - Cube_Length/2.0;
        z1 = Cube_Height/2.0f- (dm->surface[0].z * 0.001f*dt);

        for( int i = 1; i < dm->surface.size(); i++ )
        {
            x2 = (dm->surface[i].x - Min_X)*0.001f*dx - Cube_Width/2.0;
            y2 = (dm->surface[i].y - Min_Y)*0.001f*dx - Cube_Length/2.0;
            z2 = Cube_Height/2.0f- (dm->surface[i].z*0.001f*dt);


            glBegin(GL_POINTS);
    //        glBegin(GL_LINES);
    //        getColor(dm->surface[i-1].value);glColor3fv(color);
    //        glVertex3f(x1,z1,y1);
            getColor(dm->surface[i].value);glColor3fv(color);
            glVertex3f(x2,z2,y2);
            glEnd();

            x1 = x2;
            y1 = y2;
            z1 = z2;
        }
        glEndList();
        ListNum.push_back(NN);
    }
}


/*****The all timer,mouse and keyboard events******/
void GLWidget::timerEvent(QTimerEvent *)
{
    xRot += xSpeed;
    zRot += ySpeed;
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
        xRot += 90 * dy;
    }
    else if ( e->buttons() & Qt::RightButton )
    {
        ySpeed = 18 * dx;
        xSpeed = 18 * dy;
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
