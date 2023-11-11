#pragma once
#include<snbose/vertex.h>
#include<snbose/index.h>
#include<snbose/vertex_array.h>
#include<snbose/texture.h>
#include<snbose/shader.h>
#include<vector>

class Mesh{
    public:
        bool in_gpu = false;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture*> textures;

        VertexBuffer *vertex_buffer = nullptr;
        IndexBuffer *index_buffer = nullptr;
        VertexArrayBuffer vertex_array_buffer;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        Mesh(std::vector<Vertex> vertices);

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<Texture*> textures);

        void draw(ShaderProgram &shader_program);
        void bind();
        void unbind();
    private:
        void send_to_gpu();
};
