#include "shader.h"
#include<GL/glew.h>
#include<iostream>
#include<fstream>
#include<sstream>

Shader::Shader(const unsigned int type, const std::string& file_path): type(type), status(GL_FALSE){
    this->id = glCreateShader(type);
    const std::string source = this->read(file_path);
    this->compile(source);
}

Shader::~Shader(){
    glDeleteShader(this->id);
}

std::string Shader::read(const std::string& file_path) const{
    // reads shader source from file
    std::ifstream shader_stream(file_path);
    std::stringstream ss;
    std::string line;
    while(getline(shader_stream, line)){
        ss << line << "\n";
    }
    return ss.str();
}

void Shader::check_error(){
    // check for compilation error and get logs from gpu
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &this->status);
    if(this->status != GL_TRUE){
        // int loglength;
        // glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &loglength);
        // char *logs = (char*)alloca(loglength*sizeof(char));
        char logs[256];
        glGetShaderInfoLog(this->id, sizeof(logs), NULL, logs);
        std::cerr<<"SHADER ERROR: failed to compile "<< (this->type == GL_VERTEX_SHADER ? "vertex" : "fragment")<<" shader"<<std::endl;
        std::cerr<<logs<<std::endl;
    }
}

void Shader::compile(const std::string& source){
    const char* c_str = source.c_str();
    glShaderSource(this->id, 1, &c_str, nullptr);
    glCompileShader(this->id);
    this->check_error();
}

ShaderProgram::ShaderProgram(Shader vertex_shader, Shader fragment_shader){
    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader.id);
    glAttachShader(this->id, fragment_shader.id);
}

ShaderProgram::ShaderProgram(const std::string &vertex_shader_path, const std::string &fragment_shader_path){
    Shader vertex_shader(GL_VERTEX_SHADER, "shaders/basic.vs");
    Shader fragment_shader(GL_FRAGMENT_SHADER, "shaders/basic.fs");
    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader.id);
    glAttachShader(this->id, fragment_shader.id);
}

ShaderProgram::~ShaderProgram(){
    glDeleteProgram(this->id);
}

void ShaderProgram::check_error(){
    glGetProgramiv(this->id, GL_VALIDATE_STATUS, &this->status);
    if(this->status != GL_TRUE){
        // int loglength;
        // glGetProgramiv(program, GL_INFO_LOG_LENGTH, &loglength);
        // char *logs = (char*)alloca(loglength*sizeof(char));
        char logs[256];
        glGetProgramInfoLog(this->id, sizeof(logs), NULL, logs);
        std::cerr<<"SHADER_PROGRAM ERROR: failed to link program "<<std::endl;
        std::cerr<<logs<<std::endl;
    }
}

void ShaderProgram::bind(){
    glLinkProgram(this->id);
    glValidateProgram(this->id);
    this->check_error();
    glUseProgram(this->id);
}


void ShaderProgram::rebind(){
    glUseProgram(this->id);
}

void ShaderProgram::unbind(){
    glUseProgram(GL_NONE);
}

void ShaderProgram::set_uniform4f(const std::string& name, float a, float b, float c, float d){
    int location = glGetUniformLocation(this->id, name.c_str());
    // check for error in location
    glUniform4f(location, a, b, c, d);
}

void ShaderProgram::set_uniform1i(const std::string& name, const unsigned int texture_id){
    int location = glGetUniformLocation(this->id, name.c_str());
    // check for error in location
    glUniform1i(location, texture_id);
}

void ShaderProgram::set_uniformm4f(const std::string& name, const float *mat){
    int location = glGetUniformLocation(this->id, name.c_str());
    // check for error in location
    glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}