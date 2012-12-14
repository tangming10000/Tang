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
    sp = makeObjects();
}
SoSeparator* MyExaminerViewer::makeObjects()
{
    static float norms[8][3] =
    {    {0, .555, .832}, {-.832, .555, 0}, {0, .555, -.832}, { .832, .555, 0},\
         {0, .0739, .9973}, {-.9972, .0739,0},{0, .0739, -.9973}, { .9972, .0739,0} };
    const int numvertices[8] = {3, 3, 3, 3, 4, 4, 4, 4};
    static float vertices[28][3] = {
    { 0, 30, 0}, {-2,27, 2}, { 2,27, 2},
    //front
    { 0, 30, 0}, {-2,27,-2}, {-2,27, 2},
    //left
    { 0, 30, 0}, { 2,27,-2}, {-2,27,-2},
    //rear
    { 0, 30, 0}, { 2,27, 2}, { 2,27,-2},
    //right
    {-2, 27, 2}, {-4,0, 4}, { 4,0, 4}, { 2,27, 2}, //front
    {-2, 27,-2}, {-4,0,-4}, {-4,0, 4}, {-2,27, 2}, //left
    { 2, 27,-2}, { 4,0,-4}, {-4,0,-4}, {-2,27,-2}, //rear
    { 2, 27, 2}, { 4,0, 4}, { 4,0,-4}, { 2,27,-2}
    //right
    };



    SoSeparator *obelisk = new SoSeparator();
    obelisk->ref();
    // Define the normals used:
    SoNormal *myNormals = new SoNormal;
    myNormals->vector.setValues(0, 8, norms);
    obelisk->addChild(myNormals);
    SoNormalBinding *myNormalBinding = new SoNormalBinding;
    myNormalBinding->value = SoNormalBinding::PER_FACE;
    obelisk->addChild(myNormalBinding);
    // Define material for obelisk
    SoMaterial *myMaterial = new SoMaterial;
    myMaterial->diffuseColor.setValue(.4, .4, .4);
    obelisk->addChild(myMaterial);
    // Define coordinates for vertices
    SoCoordinate3 *myCoords = new SoCoordinate3;
    myCoords->point.setValues(0, 28, vertices);
    obelisk->addChild(myCoords);
    // Define the FaceSet
    SoFaceSet *myFaceSet = new SoFaceSet;
    myFaceSet->numVertices.setValues(0, 8, numvertices);
    obelisk->addChild(myFaceSet);
    obelisk->unrefNoDelete();
    return obelisk;


}
