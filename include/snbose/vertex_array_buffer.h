#pragma once

#include<snbose/vertex_buffer.h>
#include<snbose/index_buffer.h>

template <typename T>
class VertexArrayBuffer{
    private:
        unsigned int id;
        unsigned int index = 0;
        unsigned int vertex_size;
        unsigned int used_element_in_vertex = 0;
        VertexBuffer<T> *vertex_buffer;
        IndexBuffer *index_buffer;
    public:
        VertexArrayBuffer();
        ~VertexArrayBuffer();
        void bind(VertexBuffer<T> &vertex_buffer);
        void bind(IndexBuffer &index_buffer);
        void rebind();
        void unbind();
        void push(const unsigned int count);
};
