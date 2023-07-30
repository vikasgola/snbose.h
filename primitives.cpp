#include "src/shader.h"
#include "src/helper.h"
#include "src/object.h"

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

using namespace std;

int main(int argc, char const *argv[]){
    if(glfwInit() != GLFW_TRUE){
        cerr<<"ERROR: failed to initialize glfw."<<endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    GLFWwindow *window = glfwCreateWindow(800, 600, "Checker Board", NULL, NULL);
    if(!window){
        cerr<<"ERROR: failed to create a window."<<endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    // set context to new created windows
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        cerr<<"ERROR: failed to intialize glew."<<endl;
        return EXIT_FAILURE;
    }

    ShaderProgram shader_program("shaders/basic.vs", "shaders/basic.fs");


    // working with points
    glPointSize(10.0f);
    const float point_vertices[] = {
        -0.5, -0.5, 0.0
    };
    unsigned int point_layout[] = {3};
    Object<float> point(point_vertices, point_layout, 1, 1);


    // working with lines
    const float line_vertices[] = {
        0.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
    };
    unsigned int line_layout[] = {3};
    Object<float> line(line_vertices, line_layout, 1, 2);


    // GL_TRIANGLES:
    // Indices:     0 1 2 3 4 5 ...
    // Triangles:  {0 1 2}
    //                   {3 4 5}

    // GL_TRIANGLE_STRIP:
    // Indices:     0 1 2 3 4 5 ...
    // Triangles:  {0 1 2}
    //               {1 2 3}  drawing order is (2 1 3) to maintain proper winding
    //                 {2 3 4}
    //                   {3 4 5}  drawing order is (4 3 5) to maintain proper winding

    // GL_TRIANGLE_FAN:
    // Indices:     0 1 2 3 4 5 ...
    // Triangles:  {0 1 2}
    //             {0} {2 3}
    //             {0}   {3 4}
    //             {0}     {4 5}

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.05, 0.05, 0.05, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        point.bind();
        shader_program.use();
        glDrawArrays(GL_POINTS, 0, 1);

        line.bind();
        shader_program.use();
        glDrawArrays(GL_LINES, 0, 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
