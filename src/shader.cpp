#include<snbose/shader.h>
#include<GL/glew.h>
#include<fstream>
#include<sstream>

Shader::Shader(const unsigned int type, const std::string& file_path): type(type), status(GL_FALSE){
    this->id = glCreateShader(type);
    const std::string source = this->read(file_path);
    this->compile(source);
}

Shader::Shader(const unsigned int type): type(type), status(GL_FALSE){
    this->id = glCreateShader(type);
}

Shader Shader::fromSource(const unsigned int type, const std::string& shader_code){
    Shader s(type);
    s.compile(shader_code);
    return s;
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
        fprintf(stderr, "[ERROR]: %s\n", logs);
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
    glAttachShader(this->id, vertex_shader.get_id());
    glAttachShader(this->id, fragment_shader.get_id());
    this->link();
}

ShaderProgram::ShaderProgram(const std::string &vertex_shader_path, const std::string &fragment_shader_path){
    Shader vertex_shader(GL_VERTEX_SHADER, vertex_shader_path);
    Shader fragment_shader(GL_FRAGMENT_SHADER, fragment_shader_path);
    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader.get_id());
    glAttachShader(this->id, fragment_shader.get_id());
    this->link();
}

ShaderProgram ShaderProgram::fromSource(const std::string& vertex_shader_code, const std::string& fragment_shader_code){
    return ShaderProgram(
        Shader::fromSource(GL_VERTEX_SHADER, vertex_shader_code),
        Shader::fromSource(GL_FRAGMENT_SHADER, fragment_shader_code)
    );
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


void ShaderProgram::bind(){
    glUseProgram(this->id);

    for(auto &var: this->sp_vars){
        int location = this->get_location(var.first);
        if(location == -1){
            // fprintf(stderr, "[ERROR]: Couldn't locate '%s' in shader.\n", var.first.c_str());
            continue;
        }

        auto &data = var.second;
        switch (var.second.type){
            case sp_var_type::FLOAT:
                glUniform1f(location, data.float_data);
                break;
            case sp_var_type::UINT:{
                glUniform1i(location, data.unsigned_data);
                break;
            }
            case sp_var_type::VEC2:
                glUniform2fv(location, 1, data.vec2_data.array);
                break;
            case sp_var_type::VEC3:
                glUniform3fv(location, 1, data.vec3_data.array);
                break;
            case sp_var_type::VEC4:
                glUniform4fv(location, 1, data.vec4_data.array);
                break;
            case sp_var_type::MAT4x4:
                glUniformMatrix4fv(location, 1, GL_FALSE, data.mat4_data.array);
                break;
            default:
                fprintf(stderr, "[ERROR]: Don't know how to transafer var of type %d.\n", data.type);
                break;
        }
    }
}

void ShaderProgram::unbind(){
    glUseProgram(GL_NONE);
    // Clear all set shader variables to remove unexpected problems in future.
    this->sp_vars.clear();
}

int ShaderProgram::get_location(const std::string& name){
    return glGetUniformLocation(this->id, name.c_str());
}

template<typename T>
void ShaderProgram::sv(std::string name, T data){
    fprintf(stderr, "[ERROR]: NOT IMPLEMENTED! %s", name);
}

template<>
void ShaderProgram::sv<float>(std::string name, float data){
    sp_var var;
    var.float_data = data;
    var.type = sp_var_type::FLOAT;
    this->sp_vars[name] = var;
}

template<>
void ShaderProgram::sv<vec2>(std::string name, vec2 data){
    sp_var var;
    var.vec2_data = data;
    var.type = sp_var_type::VEC2;
    this->sp_vars[name] = var;
}

template<>
void ShaderProgram::sv<vec3>(std::string name, vec3 data){
    sp_var var;
    var.vec3_data = data;
    var.type = sp_var_type::VEC3;
    this->sp_vars[name] = var;
}

template<>
void ShaderProgram::sv<vec4>(std::string name, vec4 data){
    sp_var var;
    var.vec4_data = data;
    var.type = sp_var_type::VEC4;
    this->sp_vars[name] = var;
}

template<>
void ShaderProgram::sv<mat4>(std::string name, mat4 data){
    sp_var var;
    var.mat4_data = data;
    var.type = sp_var_type::MAT4x4;
    this->sp_vars[name] = var;
}

template<>
void ShaderProgram::sv<int>(std::string name, int data){
    sp_var var;
    var.int_data = data;
    var.type = sp_var_type::INT;
    this->sp_vars[name] = var;
}

template<>
void ShaderProgram::sv<unsigned>(std::string name, unsigned data){
    sp_var var;
    var.unsigned_data = data;
    var.type = sp_var_type::UINT;
    this->sp_vars[name] = var;
}

template<uint T>
void ShaderProgram::set_uniformf(const std::string& name, const vec<T> &value){
    int location = this->get_location(name);
    if(location == -1){
        fprintf(stderr, "[ERROR]: Couldn't locate '%s' in shader.\n", name.c_str());
        return;
    }
    auto gluniform = glUniform4fv;
    switch(T){
        case 1: gluniform = glUniform1fv; break;
        case 2: gluniform = glUniform2fv; break;
        case 3: gluniform = glUniform3fv; break;
        case 4: gluniform = glUniform4fv; break;
        default:
            fprintf(stderr, "[ERROR]: Uniform vec float for length %d dont exist.", T);
            return;
    }
    gluniform(location, 1, value.array);
}

void ShaderProgram::set_uniform1i(const std::string& name, const unsigned int value){
    int location = this->get_location(name);
    if(location == -1){
        fprintf(stderr, "[ERROR]: Couldn't locate '%s' in shader.\n", name.c_str());
    }
    glUniform1i(location, value);
}

void ShaderProgram::set_uniform1f(const std::string& name, const float value){
    int location = this->get_location(name);
    if(location == -1){
        fprintf(stderr, "[ERROR]: Couldn't locate '%s' in shader.\n", name.c_str());
    }
    glUniform1f(location, value);
}

void ShaderProgram::set_uniformm4f(const std::string& name, const mat4& mat){
    int location = this->get_location(name);
    if(location == -1){
        fprintf(stderr, "[ERROR]: Couldn't locate '%s' in shader.\n", name.c_str());
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, mat.array);
}

template void ShaderProgram::set_uniformf<1>(const std::string& name, const vec<1> &value);
template void ShaderProgram::set_uniformf<2>(const std::string& name, const vec<2> &value);
template void ShaderProgram::set_uniformf<3>(const std::string& name, const vec<3> &value);
template void ShaderProgram::set_uniformf<4>(const std::string& name, const vec<4> &value);
