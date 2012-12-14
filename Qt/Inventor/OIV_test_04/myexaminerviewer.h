#ifndef MYEXAMINERVIEWER_H
#define MYEXAMINERVIEWER_H

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/viewers/SoQtViewer.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/manips/SoTrackballManip.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <GL/gl.h>

class MyExaminerViewer : public SoQtExaminerViewer
{

public:
    MyExaminerViewer(QWidget * parent = 0);
    ~MyExaminerViewer();
protected:
    virtual void actualRedraw(void);
    void Draw();
private:
    SoSeparator * bckgroundroot;
    SoSeparator * foregroundroot;
    SoSeparator *sp;
    SoRotationXYZ * arrowrotation;
};
#endif // MYEXAMINERVIEWER_H
