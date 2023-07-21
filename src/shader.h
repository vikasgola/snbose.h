#pragma once
#include<string>

class Shader{
    private:
        int status;
        std::string read(const std::string& file_path) const;
        void check_error();
        void compile(const std::string& source);
    public:
        unsigned int id;
        unsigned int type;
        Shader(const unsigned int type, const std::string& file_path);
        ~Shader();
};

class ShaderProgram{
    private:
        unsigned int id;
        void check_error();
        int status;
    public:
        ShaderProgram(Shader vertex_shader, Shader fragment_shader);
        ~ShaderProgram();
        void bind();
        void rebind();
        void unbind();
        void set_uniform4f(const std::string& name, float a, float b, float c, float d);
        void set_uniform1i(const std::string& name, const unsigned int texture_id);

};