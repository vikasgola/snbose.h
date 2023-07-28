#pragma once

template <typename T>
class VertexBuffer{
    private:
        unsigned int id;
        const T *data;
        const unsigned int vertices_count;
        const unsigned int vertex_size;
    public:
        VertexBuffer(const T *data, const unsigned int vertex_size, const unsigned int vertices_count);
        ~VertexBuffer();
        void bind();
        void unbind();
        inline const unsigned int get_vertices_count() {return this->vertices_count;}
        inline const unsigned int get_vertex_size() {return this->vertex_size;}
};
