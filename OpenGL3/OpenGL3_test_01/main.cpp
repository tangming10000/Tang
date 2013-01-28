#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

using namespace glm;

int main()
{
    if ( !glfwInit() )
    {
        fprintf(stderr,"Failed to initialize GLFW!\n");
        exit(-1);
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES,4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR,3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    if( !glfwOpenWindow( 1024,768,0,0,0,0,32,0,GLFW_WINDOW))
    {
        fprintf(stderr,"Failed to open GLFW Window!\n");
        glfwTerminate();
        return -1;
    }

    glewExperimental = true;
    if ( glewInit() != GLEW_OK )
    {
        fprintf(stderr,"Failed to initilize GLEW!\n");
        return -1;
    }
    glfwSetWindowTitle("Tutorial 01");
    glfwEnable(GLFW_STICKY_KEYS );
    do
    {
        glfwSwapBuffers();
    }
    while( glfwGetKey( GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));
    return 0;
}
