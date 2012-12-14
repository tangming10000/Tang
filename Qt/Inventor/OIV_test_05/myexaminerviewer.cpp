#include "myexaminerviewer.h"

MyExaminerViewer::MyExaminerViewer(QWidget * parent)
    : SoQtExaminerViewer(parent)
{
    // Coin should not clear the pixel-buffer, so the background image
    // is not removed.
    this->setClearBeforeRender(FALSE, TRUE);

    // Set up background scenegraph with image in it.

    this->bckgroundroot = new SoSeparator;
    this->bckgroundroot->ref();

    SoOrthographicCamera * cam = new SoOrthographicCamera;
    cam->position = SbVec3f(0, 0, 1);
    cam->height = 1;
    // SoImage will be at z==0.0.
    cam->nearDistance = 0.5;
    cam->farDistance = 1.5;


    this->bckgroundroot->addChild(cam);

    // Set up foreground, overlayed scenegraph.

    this->foregroundroot = new SoSeparator;
    this->foregroundroot->ref();

    SoLightModel * lm = new SoLightModel;
    lm->model = SoLightModel::BASE_COLOR;

    SoBaseColor * bc = new SoBaseColor;
    bc->rgb = SbColor(1, 1, 0);

    cam = new SoOrthographicCamera;
    cam->position = SbVec3f(0, 0, 5);
    cam->height = 10;
    cam->nearDistance = 0;
    cam->farDistance = 10;

//    const double ARROWSIZE = 2.0;

//    SoTranslation * posit = new SoTranslation;
//    posit->translation = SbVec3f(-2.5 * ARROWSIZE, 1.5 * ARROWSIZE, 0);

//    arrowrotation = new SoRotationXYZ;
//    arrowrotation->axis = SoRotationXYZ::Z;

//    SoTranslation * offset = new SoTranslation;
//    offset->translation = SbVec3f(ARROWSIZE/2.0, 0, 0);


    this->foregroundroot->addChild(cam);
    this->foregroundroot->addChild(lm);
    this->foregroundroot->addChild(bc);
//    this->foregroundroot->addChild(posit);
//    this->foregroundroot->addChild(arrowrotation);
//    this->foregroundroot->addChild(offset);
    Draw();
    this->setSceneGraph(sp);
}
MyExaminerViewer::~MyExaminerViewer()
{
    this->bckgroundroot->unref();
    this->foregroundroot->unref();
}

void MyExaminerViewer::actualRedraw()
{
    // Must set up the OpenGL viewport manually, as upon resize
    // operations, Coin won't set it up until the SoGLRenderAction is
    // applied again. And since we need to do glClear() before applying
    // the action..
    const SbViewportRegion vp = this->getViewportRegion();
    SbVec2s origin = vp.getViewportOriginPixels();
    SbVec2s size = vp.getViewportSizePixels();
    glViewport(origin[0], origin[1], size[0], size[1]);

    const SbColor col = this->getBackgroundColor();
    glClearColor(col[0], col[1], col[2], 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render our scenegraph with the image.
    SoGLRenderAction * glra = this->getGLRenderAction();
    glra->apply(this->bckgroundroot);


    // Render normal scenegraph.
    SoQtExaminerViewer::actualRedraw();


    // Increase arrow angle with 1/1000 every frame.
//    arrowrotation->angle = arrowrotation->angle.getValue() + (0.001 / M_PI * 180);
    // Render overlay front scenegraph.
    glClear(GL_DEPTH_BUFFER_BIT);
    glra->apply(this->foregroundroot);
}

void MyExaminerViewer::Draw()
{
    sp = new SoSeparator;
    sp->ref();
    // create body parts
    // Robot with legs
    // Construct parts for legs (thigh, calf and foot)
    SoCube *thigh = new SoCube;
    thigh->width = 1.2;
    thigh->height = 2.2;
    thigh->depth = 1.1;
    SoTransform *calfTransform = new SoTransform;
    calfTransform->translation.setValue(0, -2.25, 0.0);
    SoCube *calf = new SoCube;
    calf->width = 1;calf->height = 2.2;
    calf->depth = 1;
    SoTransform *footTransform = new SoTransform;
    footTransform->translation.setValue(0, -2, .5);
    SoCube *foot = new SoCube;
    foot->width = 0.8;
    foot->height = 0.8;
    foot->depth = 2;
    // Put leg parts together
    SoGroup *leg = new SoGroup;
    leg->addChild(thigh);
    leg->addChild(calfTransform);
    leg->addChild(calf);
    leg->addChild(footTransform);
    leg->addChild(foot);
    SoTransform *leftTransform = new SoTransform;
    leftTransform->translation = SbVec3f(1, -4.25, 0);
    // Left leg
    SoSeparator *leftLeg = new SoSeparator;
    leftLeg->addChild(leftTransform);
    leftLeg->addChild(leg);
    SoTransform *rightTransform = new SoTransform;
    rightTransform->translation.setValue(-1, -4.25, 0);
    // Right leg
    SoSeparator *rightLeg = new SoSeparator;
    rightLeg->addChild(rightTransform);
    rightLeg->addChild(leg);
    // Parts for body
    SoTransform *bodyTransform = new SoTransform;
    bodyTransform->translation.setValue(0.0, 3.0, 0.0);
    SoMaterial *bronze = new SoMaterial;
    bronze->ambientColor.setValue(.33, .22, .27);
    bronze->diffuseColor.setValue(.78, .57, .11);
    bronze->specularColor.setValue(.99, .94, .81);
    bronze->shininess = .28;
    SoCylinder *bodyCylinder = new SoCylinder;
    bodyCylinder->radius = 2.5;
    bodyCylinder->height = 6;
    // Construct body out of parts
    SoSeparator *body = new SoSeparator;
    body->addChild(bodyTransform);
    body->addChild(bronze);
    body->addChild(bodyCylinder);
    body->addChild(leftLeg);
    body->addChild(rightLeg);
    // Head parts
    SoTransform *headTransform = new SoTransform;
    headTransform->translation.setValue(0, 7.5, 0);
    headTransform->scaleFactor.setValue(1.5, 1.5, 1.5);
    SoMaterial *silver = new SoMaterial;
    silver->ambientColor.setValue(.2, .2, .2);
    silver->diffuseColor.setValue(.6, .6, .6);
    silver->specularColor.setValue(.5, .5, .5);
    silver->shininess = .5;
    SoSphere *headSphere = new SoSphere;
    // Construct head
    SoSeparator *head = new SoSeparator;
    head->addChild(headTransform);
    head->addChild(silver);
    head->addChild(headSphere);
    // Robot is just head and body
    SoSeparator *robot = new SoSeparator;
    robot->addChild(body);
    robot->addChild(head);

    sp->addChild(robot);

}
