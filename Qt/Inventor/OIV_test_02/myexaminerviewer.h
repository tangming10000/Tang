#ifndef MYEXAMINERVIEWER_H
#define MYEXAMINERVIEWER_H

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/viewers/SoQtViewer.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoImage.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <GL/gl.h>

class MyExaminerViewer : public SoQtExaminerViewer
{

public:
    MyExaminerViewer(QWidget * parent, const char * filename);
    ~MyExaminerViewer();
protected:
    virtual void actualRedraw(void);

private:
    SoSeparator * bckgroundroot;
    SoSeparator * foregroundroot;
    SoRotationXYZ * arrowrotation;
};

#endif // MYEXAMINERVIEWER_H
