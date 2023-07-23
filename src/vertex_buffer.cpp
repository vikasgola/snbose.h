#include "vertex_buffer.h"
#include<GL/glew.h>

template <typename T>
VertexBuffer<T>::VertexBuffer(
    const T *data, const unsigned int vertex_size, const unsigned int vertices_count
): data(data), vertices_count(vertices_count), vertex_size(vertex_size){
    glGenBuffers(1, &this->id);
}

template <typename T>
VertexBuffer<T>::~VertexBuffer(){
    glDeleteBuffers(1, &this->id);
}

template <typename T>
void VertexBuffer<T>::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, this->vertices_count*this->vertex_size*sizeof(T), this->data, GL_STATIC_DRAW);
}

template <typename T>
void VertexBuffer<T>::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

template class VertexBuffer<float>;
