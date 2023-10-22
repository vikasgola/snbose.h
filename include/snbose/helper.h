#pragma once

#include<GL/glew.h>
#include<iostream>

GLenum glCheckError_(const char *file, int line);

#define glDebug(f) while(glGetError() != GL_NO_ERROR); f;\
    if(GLenum error = glGetError()){\
        std::cerr<<"[OPENGL ERROR] ("<<error<<"): '"<<#f<<"'"<<std::endl;\
        std::cerr<<"FILE: "<<__FILE__<<" LINE: "<<__LINE__<<std::endl;\
        exit(EXIT_FAILURE);\
    }

#define glCheckError() glCheckError_(__FILE__, __LINE__)

void init_glew();
