#pragma once
#include<string>
#include<map>
#include<advmath/advmath.h>

class Shader{
    private:
        int status;
        unsigned int id;
        unsigned int type;
        std::string read(const std::string& file_path) const;
        void check_error();
        void compile(const std::string& source);
    public:
        Shader(const unsigned int type, const std::string& file_path);
        Shader(const unsigned int type);
        ~Shader();
        static Shader fromSource(const uint type, const std::string& shader_code);

        inline uint get_id() const { return this->id;}
};

class ShaderProgram{
    private:
        unsigned int id;
        int status;
        std::map<std::string, float> sv_float;
        std::map<std::string, vec2> sv_vec2;
        std::map<std::string, vec3> sv_vec3;
        std::map<std::string, vec4> sv_vec4;

    public:
        void check_error();
        ShaderProgram(const Shader vertex_shader, const Shader fragment_shader);
        ShaderProgram(const std::string &vertex_shader_path, const std::string &fragment_shader_path);
        ~ShaderProgram();

        void sv(std::string name, float value);
        void sv(std::string name, vec2 value);
        void sv(std::string name, vec3 value);
        void sv(std::string name, vec4 value);

        static ShaderProgram fromSource(const std::string& vertex_shader_code, const std::string& fragment_shader_code);
        void link();
        void bind();
        void unbind();
        int get_location(const std::string &name);
        template<uint T>
        void set_uniformf(const std::string &name, const vec<T> &value);
        void set_uniform1i(const std::string& name, const unsigned int texture_id);
        void set_uniform1f(const std::string& name, const float texture_id);
        void set_uniformm4f(const std::string& name, const mat4& mat);

};
