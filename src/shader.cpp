#include "shader.h"
#include<GL/glew.h>
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
    if(!shader_stream.is_open()){
        fprintf(stderr, "[ERROR]: Failed to open file '%s'.\n", file_path.c_str());
    }
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
        char logs[256];
        glGetShaderInfoLog(this->id, sizeof(logs), NULL, logs);
        fprintf(stderr, "[ERROR]: Failed to compile shader '%s shader'.\n", this->type == GL_VERTEX_SHADER ? "vertex" : "fragment");
        fprintf(stderr, "[ERROR]:c %s\n", logs);
    }
}

void Shader::compile(const std::string& source){
    const char* c_str = source.c_str();
    glShaderSource(this->id, 1, &c_str, nullptr);
    glCompileShader(this->id);
    this->check_error();
}

ShaderProgram::ShaderProgram(const Shader vertex_shader, const Shader fragment_shader){
    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader.id);
    glAttachShader(this->id, fragment_shader.id);
}

ShaderProgram::ShaderProgram(const std::string &vertex_shader_path, const std::string &fragment_shader_path){
    Shader vertex_shader(GL_VERTEX_SHADER, vertex_shader_path);
    Shader fragment_shader(GL_FRAGMENT_SHADER, fragment_shader_path);
    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader.id);
    glAttachShader(this->id, fragment_shader.id);
    this->link();
}

ShaderProgram::~ShaderProgram(){
    glDeleteProgram(this->id);
}

void ShaderProgram::check_error(){
    glGetProgramiv(this->id, GL_LINK_STATUS, &this->status);
    if(this->status != GL_TRUE){
        char logs[256];
        glGetProgramInfoLog(this->id, sizeof(logs), NULL, logs);
        fprintf(stderr, "[ERROR]: Failed to link program.\n");
        fprintf(stderr, "[ERROR]: %s\n", logs);
    }
}

void ShaderProgram::link(){
    glLinkProgram(this->id);
    // glValidateProgram(this->id);
    this->check_error();
    glUseProgram(this->id);
}


void ShaderProgram::use(){
    glUseProgram(this->id);
}

void ShaderProgram::unbind(){
    glUseProgram(GL_NONE);
}

int ShaderProgram::get_location(const std::string& name){
    int location = glGetUniformLocation(this->id, name.c_str());
    if(location == -1){
        fprintf(stderr, "[ERROR]: Couldn't locate '%s' in shader.\n", name.c_str());
    }
    return location;
}

void ShaderProgram::set_uniform4f(const std::string& name, const vec4 &value){
    int location = this->get_location(name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void ShaderProgram::set_uniform1i(const std::string& name, const unsigned int value){
    int location = this->get_location(name);
    glUniform1i(location, value);
}

void ShaderProgram::set_uniformm4f(const std::string& name, const mat4& mat){
    int location = this->get_location(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, mat.array);
}
