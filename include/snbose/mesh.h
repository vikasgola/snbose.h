#pragma once
#include<snbose/vertex.h>
#include<snbose/index.h>
#include<snbose/vertex_array.h>
#include<snbose/texture.h>
#include<snbose/shader.h>
#include<vector>
#include<map>

class Mesh{
    private:
        bool in_gpu = false;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        VertexBuffer *vertex_buffer = nullptr;
        IndexBuffer *index_buffer = nullptr;
        VertexArrayBuffer vertex_array_buffer;

        std::map<std::string, vec2> sv_vec2;
        std::map<std::string, vec3> sv_vec3;
        std::map<std::string, vec4> sv_vec4;

        void send_to_gpu();
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        Mesh(std::vector<Vertex> vertices);

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);

        Mesh& operator=(const Mesh &object);

        void draw(ShaderProgram &shader_program);
        void bind();
        void unbind();
        void sv(std::string name, vec2 value);
        void sv(std::string name, vec3 value);
        void sv(std::string name, vec4 value);

        friend class Model;
};
