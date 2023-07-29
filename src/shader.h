#pragma once
#include<string>
#include<glm/glm.hpp>

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
        ShaderProgram(const Shader vertex_shader, const Shader fragment_shader);
        ShaderProgram(const std::string &vertex_shader_path, const std::string &fragment_shader_path);
        ~ShaderProgram();
        void link();
        void use();
        void unbind();
        int get_location(const std::string &name);
        void set_uniform4f(const std::string &name, const glm::vec4 &value);
        void set_uniform1i(const std::string& name, const unsigned int texture_id);
        void set_uniformm4f(const std::string& name, const float *mat);

};
