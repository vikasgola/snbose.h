#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

#define GLDebug(f) while(glGetError() != GL_NO_ERROR); f;\
    if(GLenum error = glGetError()){\
        std::cerr<<"OPENGL ERROR ("<<error<<"): '"<<#f<<"'"<<std::endl;\
        std::cerr<<"FILE: "<<__FILE__<<" LINE: "<<__LINE__<<std::endl;\
        glfwTerminate();\
        exit(EXIT_FAILURE);\
    }
