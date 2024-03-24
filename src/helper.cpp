#include<snbose/helper.h>

GLenum glCheckError_(const char *file, int line){
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR){
        std::string error;
        switch (errorCode){
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        fprintf(stderr, "[ERROR] (%s | %d): %s | %s.\n", file, line, error.c_str(), glewGetErrorString(errorCode));
        exit(EXIT_FAILURE);
    }
    return errorCode;
}

void init_glew(){
    // initialize the opengl
    if(glewInit() != GLEW_OK){
        std::cerr<<"[ERROR]: failed to intialize glew."<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout<<"[INFO]: GLEW successfully initialized!"<<std::endl;
    std::cout<<"[INFO]: OpenGL Version - "<<glGetString(GL_VERSION)<<std::endl;
    glEnable(GL_DEPTH_TEST);
}
