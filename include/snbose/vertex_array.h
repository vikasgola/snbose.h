#pragma once

#include<snbose/vertex.h>
#include<snbose/index.h>

class VertexArrayBuffer{
    private:
        unsigned int id;
        unsigned int index = 0;
        unsigned int vertex_size;
        unsigned int used_element_in_vertex = 0;
        VertexBuffer *vertex_buffer = nullptr;
        IndexBuffer *index_buffer = nullptr;
        void set_pointers();
    public:
        VertexArrayBuffer();
        ~VertexArrayBuffer();
        void bind(VertexBuffer &vertex_buffer);
        void bind(IndexBuffer &index_buffer);
        void rebind();
        void unbind();
};
