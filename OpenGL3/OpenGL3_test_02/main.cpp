#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <QObject>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

using namespace glm;

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
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

    glClearColor(0.0f,0.0f,0.4f,0.0f);
    GLuint VertexArrayID;
    glGenVertexArrays(1,&VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader","SimpleFragmentShader.fragmentshader");
    static const GLfloat g_vertex_buffer_data[] =
    {
        -1.0f,-1.0f,+0.0f,
        +1.0f,-1.0f,+0.0f,
        +0.0f,+1.0f,+0.0f,
        //+2.0f,+1.0f,+0.0f
    };

    GLuint vertexbuffer;
    glGenBuffers(1,&vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof( g_vertex_buffer_data),g_vertex_buffer_data, GL_STATIC_DRAW);

    do
    {
        glClear( GL_COLOR_BUFFER_BIT );
        glUseProgram(programID);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                    );
        glDrawArrays( GL_TRIANGLES , 0, 3 );
        glDisableVertexAttribArray(0);
        glfwSwapBuffers();
    }
    while( glfwGetKey( GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));
    glDeleteVertexArrays(1,&VertexArrayID);
    glDeleteBuffers(1,&vertexbuffer);

    return 0;
}


GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    //read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path,std::ios_base::in);
    if( VertexShaderStream.is_open())
    {
        std::string Line = "";
        while ( getline( VertexShaderStream,Line ))
        {
            VertexShaderCode += "\n" + Line;
        }
        VertexShaderStream.close();
    }

    //read the fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream( fragment_file_path, std::ios_base::in );
    if( FragmentShaderStream.is_open() )
    {
        std::string Line = "";
        while( getline( FragmentShaderStream, Line ))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    //compile Vertex Shader
    printf("Compiling Vertex Shader file:%s.\n", vertex_file_path );
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer,NULL);
    glCompileShader(VertexShaderID);

    //check the Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS,&Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID,InfoLogLength,NULL,&VertexShaderErrorMessage[0]);
    fprintf(stdout,"%s.\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Link the program
    fprintf(stdout,"Linking program!!\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    //check the program
    glGetProgramiv(ProgramID,GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( max(InfoLogLength,int(1)));
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout,"%s.\n",&ProgramErrorMessage[0]);
    glDeleteShader( VertexShaderID );
    glDeleteShader( FragmentShaderID );
    return ProgramID;


}
