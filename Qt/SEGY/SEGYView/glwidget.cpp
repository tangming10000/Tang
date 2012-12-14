#include "glwidget.h"

GLWidget::GLWidget(MainWindow *parent) :
    QGLWidget(parent)
{
    setMinimumSize(640,480);
    setFocusPolicy(Qt::StrongFocus);
    main = parent;
    MReel = main->Reel;
    //mainWindow = parent;
    mx = 0.0;
    my = 0.0;
    zoom = -1.0;
    isDraw = false; //画图
    isDrawGrid = true;
    sx = 0.002;  //X间隔
    st = 0.00075;  //t间隔
    Num = 0;
    Name = "";
    xRot=yRot=zRot=0.0;
    Fp = 3840;
    LNO = 1;
    DrawType = 0;   //画图形式
    mouseUse = 0;   //鼠标作用
    PointSize = 0.1;
}
GLWidget::~GLWidget()
{
    for (int m = 0; m < Num; m++)
    {
        delete[] DATA[m];
    }
    delete[] DATA;
}

void GLWidget::initializeGL()
{

    glShadeModel(GL_SMOOTH);
    glClearColor(1.0,1.0,1.0,0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    //startTimer(300);
}
void GLWidget::resizeGL(int w, int h)
{
    if ( h == 0)
        h = 1;
    glViewport(0,0,(GLint)w,(GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0,(GLfloat)w/(GLfloat)h,0.001,1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Left:
        xRot -= 10;
        break;
    case Qt::Key_Right:
        xRot += 10;
        break;
    case Qt::Key_Up:
        yRot += 10;
        break;
    case Qt::Key_Down:
        yRot -= 10;
        break;
    case Qt::Key_PageUp:
        zRot += 10;
        break;
    case Qt::Key_PageDown:
        zRot -= 10;
        break;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    lasPos = e->pos();
    switch(mouseUse)
    {
    case 0:
        setCursor(Qt::ClosedHandCursor);
        break;
    }
}
void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    GLfloat dx = GLfloat(e->x() - lasPos.x())/width();
    GLfloat dy = GLfloat(e->y() - lasPos.y())/height();
    switch(mouseUse)
    {
    case 0:/*图像左右移动*/
        if ( e->buttons() & Qt::LeftButton )
        {
            mx -= dx*zoom;
            my += dy*zoom;
            updateGL();
        }
        break;
    case 1:
        if ( e->buttons() & Qt::LeftButton )
        {
            xRot += 180*dy;
            yRot += 180*dx;
            updateGL();
        }
        else if( e->buttons() & Qt::RightButton )
        {
            xRot += 180*dy;
            zRot += 180*dx;
            updateGL();
        }

        break;
    }
    lasPos = e->pos();
}
void GLWidget::mouseReleaseEvent(QMouseEvent *)
{
    switch(mouseUse)
    {
    case 0:
        setCursor(Qt::OpenHandCursor);
        break;
    }
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    e->delta() > 0 ? zoom += 0.1*zoom : zoom -= zoom*0.1;
    updateGL();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    switch( mouseUse )
    {
    case 0:
        DrawType += 1;
        if( DrawType  >= 3 )
            DrawType = 0;
        updateGL();
        break;
    case 1:
        xRot=yRot=zRot=0.0;
        updateGL();
        break;
    }
}

void GLWidget::SetData()
{
    DATA = new float*[Num];
    for(int i =0;i<Num;i++)
        DATA[i]=new float[MReel.hns];

    QFile Readfile(Name);
    Readfile.open(QIODevice::ReadOnly);
    QDataStream fs(&Readfile);
    fs.setByteOrder(QDataStream::BigEndian);
    qint64 fp;

    for(int i = 0; i <Num;i++)
    {
        float temp[MReel.hns];
        fp = Fp  + double(i)*(MReel.hns*MReel.DataType + 240.0);
        fs.device()->seek(fp);
        ReadDataATP(fs,temp,MReel.hns,MReel.format);
        for(int j =0; j<MReel.hns;j++)
            DATA[i][j]=temp[j];
    }
}

void GLWidget::changeSectionType(int sectiontype)
{
    DrawType = sectiontype;
    updateGL();
}

void GLWidget::Use_RotateX(int RX)
{
    xRot = RX;
    updateGL();
}
void GLWidget::Use_RotateY(int RY)
{
    yRot = RY;
    updateGL();
}
void GLWidget::Use_RotateZ(int RZ)
{
    zRot = RZ;
    updateGL();
}

void GLWidget::bigger()
{
    zoom -= zoom *0.2;
    if(zoom >= -.01)
        zoom = -0.01;

    updateGL();
}
void GLWidget::little()
{
    zoom += zoom*0.2;

    updateGL();
}
void GLWidget::moveleft()
{
    mx += zoom*0.2;
    updateGL();
}
void GLWidget::moveright()
{
    mx -= zoom*0.2;
    updateGL();
}
void GLWidget::moveup()
{
    my -= zoom*0.2;
    updateGL();
}
void GLWidget::movedown()
{
    my += zoom*0.2;
    updateGL();
}
void GLWidget::Chang_Mouse_Type(int mouseType)
{
    mouseUse = mouseType;
    switch(mouseUse)
    {
    case 0:
        setCursor(Qt::OpenHandCursor);
        break;
    case 1:
        setCursor(Qt::ArrowCursor);
        break;
    }

    updateGL();
}

void GLWidget::timerEvent(QTimerEvent *e)
{
    xRot += 5;
    //yRot += 5;
    //zRot += 5;
    updateGL();
}
void GLWidget::setInLine(qint64 fp, int TraceNum, int LineNO)
{
    Fp = fp;
    Num = TraceNum;
    LNO=LineNO;
    SetData();
    updateGL();
}
void GLWidget::paintGL()
{
   // MyDraw();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(mx,my,zoom);
    glRotatef(xRot,0.0,0.0,1.0);
    glRotatef(yRot,0.0,1.0,0.0);
    glRotatef(zRot,1.0,0.0,0.0);

    glPushMatrix();
    //glLoadIdentity();
    if(isDraw && main->CubeType==tr("二维数据体"))
        Draw2D();
    else if(isDraw && main->CubeType==tr("三维数据体"))
        Draw3D();
    glPopMatrix();
    swapBuffers();

}
void GLWidget::Draw2D()
{
    glTranslatef(0.0,0.0,0.0);
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
     glVertex3f(-sx*(Num/2.0+1),(MReel.hns/2.0+1)*st,0.0);
     glVertex3f( (Num/2.0)*sx,(MReel.hns/2.0+1)*st,0.0);
     glVertex3f( (Num/2.0*sx),-(MReel.hns/2.0)*st,0.0);
     glVertex3f(-sx*(Num/2.0+1),-(MReel.hns/2.0)*st,0.0);
    glEnd();


    glColor3f(0.0,0.0,0.0);
    for( int i = 0 ; i <= int(Num / 50); i ++ )
        renderText( i * 50.0 * sx+-sx*(Num/2.0+1),0.005+(MReel.hns/2.0+1)*st,0.0,QString::number(i*50),QFont("Monospace",8,QFont::Bold));
    for( int i = 1 ; i <= int(MReel.hns / 100); i ++ )
        renderText( -0.04-sx*(Num/2.0+1),-i * 100.0 * st+(MReel.hns/2.0+1)*st,0.0,QString::number(i*100),QFont("Monospace",8,QFont::Bold));
    renderText(sx*(-100), (MReel.hns/2.0+1)*st+0.05,0.0,tr("二维地震剖面"),QFont("Monospace",10,QFont::Normal));
    glColor3f(0.0,0.0,0.0);
    glLineWidth(0.2);
    for( int i = 0; i < Num; i ++)
    {
        double Max = 0;
        double Min = 0;
        float Data[MReel.hns];
        for(int j = 0; j <MReel.hns;j++)
            Data[j]=DATA[i][j];

        for(int j = 0; j < MReel.hns;j++)
        {
            if(Max < Data[j])
                Max = Data[j];
            if(Min > Data[j])
                Min = Data[j];
        }
        glColor3f(0.0,0.0,0.0);
        if (DrawType == 0)
            draw_wave(Data,Max,MReel.hns,i);
        else if ( DrawType == 1)
            draw_gray(Data,Max,Min,MReel.hns,i);
        else if (DrawType == 2)
            draw_color(Data,Max,Min,MReel.hns,i);
    }
    if(isDrawGrid)
    {
        glPointSize(1.0);
        glColor3f(1.0,0.0,1.0);
        for(int i = 1; i <= int(Num/100); i++)
        {
            glBegin(GL_LINES);
            glVertex3f(-sx*(Num/2.0-i*100),-(MReel.hns/2.0)*st,0.0);
            glVertex3f(-sx*(Num/2.0-i*100),(MReel.hns/2.0+1)*st,0.0);
            glEnd();
        }
        for(int i = 1; i <= int(MReel.hns/100); i++)
        {
            glBegin(GL_LINES);
            glVertex3f(-sx*(Num/2.0+1),(i*100- MReel.hns/2)*st,0.0);
            glVertex3f((Num/2.0)*sx,(i*100- MReel.hns/2)*st,0.0);
            glEnd();
        }
    }

}

void GLWidget::Draw3D()
{
    glTranslatef (0.0, 0.0, -zoom);
    glLineWidth(2.0);

    /*画外部的立方体*/
    int x_length = (main->Max_XLine - main->Min_XLine);
    int y_length = (main->Max_YLine - main->Min_YLine);
    GLfloat x_min,x_max,y_min,y_max,t_max,t_min;
    x_min = -sx * (x_length/2.0+1);
    x_max =  sx * (x_length/2.0);
    y_min = -sx * (y_length/2.0+1);
    y_max =  sx * (y_length/2.0);
    t_max =  st * (MReel.hns/2.0+1);
    t_min = -st * (MReel.hns/2.0);
    glColor3f(0.0,0.0,1.0);

    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_min,zoom+x_min);
      glVertex3f(y_min,t_max,zoom+x_min);
      glVertex3f(y_max,t_max,zoom+x_min);
      glVertex3f(y_max,t_min,zoom+x_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_min,zoom+x_max);
      glVertex3f(y_min,t_max,zoom+x_max);
      glVertex3f(y_max,t_max,zoom+x_max);
      glVertex3f(y_max,t_min,zoom+x_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_min,zoom+x_min);
      glVertex3f(y_min,t_max,zoom+x_min);
      glVertex3f(y_max,t_max,zoom+x_min);
      glVertex3f(y_max,t_min,zoom+x_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_min,zoom+x_max);
      glVertex3f(y_min,t_max,zoom+x_max);
      glVertex3f(y_max,t_max,zoom+x_max);
      glVertex3f(y_max,t_min,zoom+x_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_min,zoom+x_min);
      glVertex3f(y_min,t_min,zoom+x_max);
      glVertex3f(y_max,t_min,zoom+x_max);
      glVertex3f(y_max,t_min,zoom+x_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_max,zoom+x_min);
      glVertex3f(y_min,t_max,zoom+x_max);
      glVertex3f(y_max,t_max,zoom+x_max);
      glVertex3f(y_max,t_max,zoom+x_min);
    glEnd();

    x_slice_pos = zoom+x_min + (LNO-main->Min_XLine)*sx;//*计算剖面所在的位置*//
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
     glVertex3f(-sx*(Num/2.0+1),(MReel.hns/2.0+1)*st,x_slice_pos);
     glVertex3f( (Num/2.0)*sx,(MReel.hns/2.0+1)*st,x_slice_pos);
     glVertex3f( (Num/2.0*sx),-(MReel.hns/2.0)*st,x_slice_pos);
     glVertex3f(-sx*(Num/2.0+1),-(MReel.hns/2.0)*st,x_slice_pos);
    glEnd();


    glColor3f(0.0,0.0,0.0);
    for( int i = 0 ; i <= int(Num / 50); i ++ )
        renderText( i * 50.0 * sx+-sx*(Num/2.0+1),0.005+(MReel.hns/2.0+1)*st,x_slice_pos,QString::number(i*50),QFont("Monospace",8,QFont::Bold));
    for( int i = 1 ; i <= int(MReel.hns / 100); i ++ )
        renderText( -0.04-sx*(Num/2.0+1),-i * 100.0 * st+(MReel.hns/2.0+1)*st,x_slice_pos,QString::number(i*100),QFont("Monospace",8,QFont::Bold));
    renderText(sx*(-100), (MReel.hns/2.0+1)*st+0.05,x_slice_pos,tr("第%1条地震剖面").arg(QString::number(LNO)),QFont("Monospace",10,QFont::Normal));
    glColor3f(0.0,0.0,0.0);
    glLineWidth(0.2);
    for( int i = 0; i < Num; i ++)
    {
        double Max = 0;
        double Min = 0;
        float Data[MReel.hns];
        for(int j = 0; j <MReel.hns;j++)
            Data[j]=DATA[i][j];

        for(int j = 0; j < MReel.hns;j++)
        {
            if(Max < Data[j])
                Max = Data[j];
            if(Min > Data[j])
                Min = Data[j];
        }
        glColor3f(0.0,0.0,0.0);
        if (DrawType == 0)
            draw_wave(Data,Max,MReel.hns,i);
        else if ( DrawType == 1)
            draw_gray(Data,Max,Min,MReel.hns,i);
        else if (DrawType == 2)
            draw_color(Data,Max,Min,MReel.hns,i);
    }
    if(isDrawGrid)
    {
        glPointSize(1.0);
        glColor3f(1.0,0.0,1.0);
        for(int i = 1; i <= int(Num/100); i++)
        {
            glBegin(GL_LINES);
            glVertex3f(-sx*(Num/2.0-i*100),-(MReel.hns/2.0)*st,x_slice_pos);
            glVertex3f(-sx*(Num/2.0-i*100),(MReel.hns/2.0+1)*st,x_slice_pos);
            glEnd();
        }
        for(int i = 1; i <= int(MReel.hns/100); i++)
        {
            glBegin(GL_LINES);
            glVertex3f(-sx*(Num/2.0+1),(i*100- MReel.hns/2)*st,x_slice_pos);
            glVertex3f((Num/2.0)*sx,(i*100- MReel.hns/2)*st,x_slice_pos);
            glEnd();
        }
    }
}

void GLWidget::Draw()
{
    glTranslatef (0.0, 0.0, -zoom);
    glLineWidth(2.0);

    /*画外部的立方体*/
    int x_length = (main->Max_XLine - main->Min_XLine);
    int y_length = (main->Max_YLine - main->Min_YLine);
    GLfloat x_min,x_max,y_min,y_max,t_max,t_min;
    x_min = -sx * (x_length/2.0+1);
    x_max =  sx * (x_length/2.0);
    y_min = -sx * (y_length/2.0+1);
    y_max =  sx * (y_length/2.0);
    t_max =  st * (MReel.hns/2.0+1);
    t_min = -st * (MReel.hns/2.0);

    glColor3f(0.0,0.0,1.0);

    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_min,zoom+x_min);
      glVertex3f(y_min,t_max,zoom+x_min);
      glVertex3f(y_max,t_max,zoom+x_min);
      glVertex3f(y_max,t_min,zoom+x_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_min,zoom+x_max);
      glVertex3f(y_min,t_max,zoom+x_max);
      glVertex3f(y_max,t_max,zoom+x_max);
      glVertex3f(y_max,t_min,zoom+x_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_min,zoom+x_min);
      glVertex3f(y_min,t_max,zoom+x_min);
      glVertex3f(y_max,t_max,zoom+x_min);
      glVertex3f(y_max,t_min,zoom+x_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_min,zoom+x_max);
      glVertex3f(y_min,t_max,zoom+x_max);
      glVertex3f(y_max,t_max,zoom+x_max);
      glVertex3f(y_max,t_min,zoom+x_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_min,zoom+x_min);
      glVertex3f(y_min,t_min,zoom+x_max);
      glVertex3f(y_max,t_min,zoom+x_max);
      glVertex3f(y_max,t_min,zoom+x_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
      glVertex3f(y_min,t_max,zoom+x_min);
      glVertex3f(y_min,t_max,zoom+x_max);
      glVertex3f(y_max,t_max,zoom+x_max);
      glVertex3f(y_max,t_max,zoom+x_min);
    glEnd();

    //x_slice_pos = zoom+x_min + (LNO-main->Min_XLine)*sx;//*计算剖面所在的位置*//
    x_slice_pos = zoom + (LNO-2*main->Min_XLine)*sx;//*计算剖面所在的位置*//
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
     glVertex3f(-sx*(Num/2.0+1),(MReel.hns/2.0+1)*st,x_slice_pos);
     glVertex3f( (Num/2.0)*sx,(MReel.hns/2.0+1)*st,x_slice_pos);
     glVertex3f( (Num/2.0*sx),-(MReel.hns/2.0)*st,x_slice_pos);
     glVertex3f(-sx*(Num/2.0+1),-(MReel.hns/2.0)*st,x_slice_pos);
    glEnd();


    glColor3f(0.0,0.0,0.0);
    for( int i = 0 ; i <= int(Num / 50); i ++ )
        renderText( i * 50.0 * sx+-sx*(Num/2.0+1),0.005+(MReel.hns/2.0+1)*st,x_slice_pos,QString::number(i*50),QFont("Monospace",5+int(0.2/abs(x_slice_pos)),QFont::Bold));
    for( int i = 1 ; i <= int(MReel.hns / 100); i ++ )
        renderText( -0.04-sx*(Num/2.0+1),-i * 100.0 * st+(MReel.hns/2.0+1)*st,x_slice_pos,QString::number(i*100),QFont("Monospace",5+int(0.2/abs(x_slice_pos)),QFont::Bold));
    renderText(sx*(-100), (MReel.hns/2.0+1)*st+0.05,x_slice_pos,tr("第%1条地震剖面").arg(QString::number(LNO)),QFont("Monospace",10,QFont::Normal));
    glColor3f(0.0,0.0,0.0);
    glLineWidth(0.2);
    for( int i = 0; i < Num; i ++)
    {
        double Max = 0;
        double Min = 0;
        float Data[MReel.hns];
        //double fp = 3600.0 + 240.0 + double(i) * (double)(MReel.hns*MReel.DataType + 240.0);

        /*读取数据*/
        //fp = Fp  + double(i)*(MReel.hns*MReel.DataType + 240.0);
        //fs.device()->seek(fp);
        //ReadDataATP(fs,Data,MReel.hns,MReel.format);
        /*读取数据完成*/
        for(int j = 0; j <MReel.hns;j++)
            Data[j]=DATA[i][j];

        for(int j = 0; j < MReel.hns;j++)
        {
            if(Max < Data[j])
                Max = Data[j];
            if(Min > Data[j])
                Min = Data[j];
        }
        glColor3f(0.0,0.0,0.0);
        if (DrawType == 0)
            draw_wave(Data,Max,MReel.hns,i);
        else if ( DrawType == 1)
            draw_gray(Data,Max,Min,MReel.hns,i);
        else if (DrawType == 2)
            draw_color(Data,Max,Min,MReel.hns,i);
    }
    if(isDrawGrid)
    {
        glPointSize(1.0);
        glColor3f(1.0,0.0,1.0);
        for(int i = 1; i <= int(Num/100); i++)
        {
            glBegin(GL_LINES);
            glVertex3f(-sx*(Num/2.0-i*100),-(MReel.hns/2.0)*st,x_slice_pos);
            glVertex3f(-sx*(Num/2.0-i*100),(MReel.hns/2.0+1)*st,x_slice_pos);
            glEnd();
        }
        for(int i = 1; i <= int(MReel.hns/100); i++)
        {
            glBegin(GL_LINES);
            glVertex3f(-sx*(Num/2.0+1),(i*100- MReel.hns/2)*st,x_slice_pos);
            glVertex3f((Num/2.0)*sx,(i*100- MReel.hns/2)*st,x_slice_pos);
            glEnd();
        }
    }

}

void GLWidget::draw_wave(float Data[], float Max, int N, int x)
{
    glBegin(GL_LINE_STRIP);
    for( int k1 = 0; k1 < N; k1++)
    {
        Data[k1] = Data[k1]/Max/600.0*1.5;
        //Data[k1] = Data[k1]-sx*(Num/2.0);
        glVertex3f( GLfloat((x-Num/2.0) * sx + Data[k1]),-GLfloat((k1-MReel.hns/2.0) * st),x_slice_pos);
    }
    glEnd();

    for( int k = 0; k < N; k++ )
    {
        if( Data[k] <= 0 && Data[k+1] > 0)
        {
            double c1 = st * Data[k] /( Data[k] - Data[k+1]);
            glBegin(GL_POLYGON);
             glVertex3f( GLfloat((x-Num/2.0) * sx),-GLfloat((k-MReel.hns/2.0)*st +c1),x_slice_pos);
            int l = 0;
            do
            {
                l +=1;
                glVertex3f( GLfloat((x-Num/2.0) * sx + Data[k+l]),-GLfloat((k+l-MReel.hns/2.0) * st),x_slice_pos);

            }while (Data[k+l+1] > 0);

            double c2 = st * Data[k+l] /( Data[k+l] - Data[k+l+1]);

            glVertex3f( GLfloat((x-Num/2.0) * sx),-GLfloat((k+l-MReel.hns/2.0)*st+c2),x_slice_pos);
            glEnd();
            k = k + l;
        }
    }

}
void GLWidget::draw_color(float Data[], float Max, float Min, int N, int x)
{
    float r,g,b;
    for(int j = 0; j < N;j++)
    {
        r=g=b=0;
        float temp = Data[j];
        if( temp < 0)
        {
            b = 1.0-temp/Min;
            b = b*b*b;
            g = b;
            r = 1.0;
/*            if(b < 0.1)
            {
                float trg = b * 5.0;
                r = 0.5-trg;
                g = 0.5-trg;
                b = 0.0;
            }
            else
            {
                r = 0.0;
                g = 0.0;
                b = b * 10.0/9.0;
            }*/

        }
        else if(temp >= 0)
        {
            r = 1.0-temp /Max;
            r = r*r*r;
            g = r;
            b = 1.0;
            /*if ( r < 0.1 )
            {
                float trg = r * 5.0;
                r = 0.5+trg;
                g = 0.5+trg;
                b = 0.0;
            }
            else
            {
                r = r * 10.0/9.0;
                g = 0.0;
                b = 0.0;
            }*/
        }
        glColor3f(r,g,b);
        glBegin(GL_QUADS);
        glVertex3f( GLfloat((x-Num/2.0) * sx -0.5*sx),-GLfloat((j-N/2.0) * st),x_slice_pos);
        glVertex3f( GLfloat((x-Num/2.0) * sx +0.5*sx),-GLfloat((j-N/2.0) * st),x_slice_pos);
        glVertex3f( GLfloat((x-Num/2.0) * sx +0.5*sx),-GLfloat((1+j-N/2.0) * st),x_slice_pos);
        glVertex3f( GLfloat((x-Num/2.0) * sx -0.5*sx),-GLfloat((1+j-N/2.0) * st),x_slice_pos);
        glEnd();
    }

}
void GLWidget::draw_gray(float Data[], float Max, float Min, int N, int x)
{
    float gray;
    for(int j = 0; j < N;j++)
    {
        float temp = Data[j];
        if ( temp < 0)
            temp =0.5-temp/ Min/2.0;
        else
            temp = 0.5+temp/Max/2.0;
        gray = temp*temp;
        glColor3f(gray,gray,gray);
        glBegin(GL_QUADS);
        glVertex3f( GLfloat((x-Num/2.0) * sx -0.5*sx),-GLfloat((j-N/2.0) * st),x_slice_pos);
        glVertex3f( GLfloat((x-Num/2.0) * sx +0.5*sx),-GLfloat((j-N/2.0) * st),x_slice_pos);
        glVertex3f( GLfloat((x-Num/2.0) * sx +0.5*sx),-GLfloat((1+j-N/2.0) * st),x_slice_pos);
        glVertex3f( GLfloat((x-Num/2.0) * sx -0.5*sx),-GLfloat((1+j-N/2.0) * st),x_slice_pos);
        glEnd();
    }

}
void GLWidget::DrawGrid()
{
    if(isDrawGrid == true)
        isDrawGrid = false;
    else
        isDrawGrid = true;
    updateGL();
}
void GLWidget::XLineSlice()
{
    x_slice_pos = zoom + LNO*sx;//*计算剖面所在的位置*//
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
     glVertex3f(-sx*(Num/2.0+1),(MReel.hns/2.0+1)*st,x_slice_pos);
     glVertex3f( (Num/2.0)*sx,(MReel.hns/2.0+1)*st,x_slice_pos);
     glVertex3f( (Num/2.0*sx),-(MReel.hns/2.0)*st,x_slice_pos);
     glVertex3f(-sx*(Num/2.0+1),-(MReel.hns/2.0)*st,x_slice_pos);
    glEnd();


    glColor3f(0.0,0.0,0.0);
    for( int i = 0 ; i <= int(Num / 50); i ++ )
        renderText( i * 50.0 * sx+-sx*(Num/2.0+1),0.005+(MReel.hns/2.0+1)*st,x_slice_pos,QString::number(i*50),QFont("Monospace",5+int(0.2/abs(x_slice_pos)),QFont::Bold));
    for( int i = 1 ; i <= int(MReel.hns / 100); i ++ )
        renderText( -0.04-sx*(Num/2.0+1),-i * 100.0 * st+(MReel.hns/2.0+1)*st,x_slice_pos,QString::number(i*100),QFont("Monospace",5+int(0.2/abs(x_slice_pos)),QFont::Bold));
    renderText(sx*(-100), (MReel.hns/2.0+1)*st+0.05,x_slice_pos,tr("第%1条地震剖面").arg(QString::number(LNO)),QFont("Monospace",10,QFont::Normal));
    glColor3f(0.0,0.0,0.0);
    glLineWidth(0.2);
    for( int i = 0; i < Num; i ++)
    {
        double Max = 0;
        double Min = 0;
        float Data[MReel.hns];

        for(int j = 0; j <MReel.hns;j++)
            Data[j]=DATA[i][j];

        for(int j = 0; j < MReel.hns;j++)
        {
            if(Max < Data[j])
                Max = Data[j];
            if(Min > Data[j])
                Min = Data[j];
        }
        glColor3f(0.0,0.0,0.0);
        if (DrawType == 0)
            draw_wave(Data,Max,MReel.hns,i);
        else if ( DrawType == 1)
            draw_gray(Data,Max,Min,MReel.hns,i);
        else if (DrawType == 2)
            draw_color(Data,Max,Min,MReel.hns,i);
    }
    if(isDrawGrid)
    {
        glPointSize(1.0);
        glColor3f(1.0,0.0,1.0);
        for(int i = 1; i <= int(Num/100); i++)
        {
            glBegin(GL_LINES);
            glVertex3f(-sx*(Num/2.0-i*100),-(MReel.hns/2.0)*st,x_slice_pos);
            glVertex3f(-sx*(Num/2.0-i*100),(MReel.hns/2.0+1)*st,x_slice_pos);
            glEnd();
        }
        for(int i = 1; i <= int(MReel.hns/100); i++)
        {
            glBegin(GL_LINES);
            glVertex3f(-sx*(Num/2.0+1),(i*100- MReel.hns/2)*st,x_slice_pos);
            glVertex3f((Num/2.0)*sx,(i*100- MReel.hns/2)*st,x_slice_pos);
            glEnd();
        }
    }

}
void GLWidget::YLineSlice(int CrossLine, int fpointer[], int Num)
{
    /*画CrossLine边框*/
    x_slice_pos = zoom + LNO*sx;//*计算剖面所在的位置*//
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
     glVertex3f(-sx*(Num/2.0+1),(MReel.hns/2.0+1)*st,x_slice_pos);
     glVertex3f( (Num/2.0)*sx,(MReel.hns/2.0+1)*st,x_slice_pos);
     glVertex3f( (Num/2.0*sx),-(MReel.hns/2.0)*st,x_slice_pos);
     glVertex3f(-sx*(Num/2.0+1),-(MReel.hns/2.0)*st,x_slice_pos);
    glEnd();
    for( int i = 0; i < Num; i ++)
    {
        double Max = 0;
        double Min = 0;
        float Data[MReel.hns];
        //double fp = 3600.0 + 240.0 + double(i) * (double)(MReel.hns*MReel.DataType + 240.0);

        /*读取数据*/
        //fp = Fp  + double(i)*(MReel.hns*MReel.DataType + 240.0);
        //fs.device()->seek(fp);
        //ReadDataATP(fs,Data,MReel.hns,MReel.format);
        /*读取数据完成*/
        for(int j = 0; j <MReel.hns;j++)
            Data[j]=DATA[i][j];

        for(int j = 0; j < MReel.hns;j++)
        {
            if(Max < Data[j])
                Max = Data[j];
            if(Min > Data[j])
                Min = Data[j];
        }
        glColor3f(0.0,0.0,0.0);
        if (DrawType == 0)
            draw_wave(Data,Max,MReel.hns,i);
        else if ( DrawType == 1)
            draw_gray(Data,Max,Min,MReel.hns,i);
        else if (DrawType == 2)
            draw_color(Data,Max,Min,MReel.hns,i);
    }
    if(isDrawGrid)
    {
        glPointSize(1.0);
        glColor3f(1.0,0.0,1.0);
        for(int i = 1; i <= int(Num/100); i++)
        {
            glBegin(GL_LINES);
            glVertex3f(-sx*(Num/2.0-i*100),-(MReel.hns/2.0)*st,x_slice_pos);
            glVertex3f(-sx*(Num/2.0-i*100),(MReel.hns/2.0+1)*st,x_slice_pos);
            glEnd();
        }
        for(int i = 1; i <= int(MReel.hns/100); i++)
        {
            glBegin(GL_LINES);
            glVertex3f(-sx*(Num/2.0+1),(i*100- MReel.hns/2)*st,x_slice_pos);
            glVertex3f((Num/2.0)*sx,(i*100- MReel.hns/2)*st,x_slice_pos);
            glEnd();
        }
    }

}

void GLWidget::TimeSlice()
{

}
