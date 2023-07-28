#include "vertex_array_buffer.h"
#include<GL/glew.h>

template <typename T>
VertexArrayBuffer<T>::VertexArrayBuffer(){
    glGenVertexArrays(1, &this->id);
}

template <typename T>
VertexArrayBuffer<T>::~VertexArrayBuffer(){
    glDeleteBuffers(1, &this->id);
}

template <typename T>
void VertexArrayBuffer<T>::bind(VertexBuffer<T> &vertex_buffer){
    this->rebind();
    this->vertex_buffer = &vertex_buffer;
    this->vertex_buffer->bind();
}

template <typename T>
void VertexArrayBuffer<T>::bind(IndexBuffer &index_buffer){
    this->rebind();
    this->index_buffer = &index_buffer;
    this->index_buffer->bind();
}

template <typename T>
void VertexArrayBuffer<T>::rebind(){
    glBindVertexArray(this->id);
}

template <typename T>
void VertexArrayBuffer<T>::unbind(){
    glBindVertexArray(GL_NONE);
    this->vertex_buffer->unbind();
    this->index_buffer->unbind();
}
template <typename T>
void VertexArrayBuffer<T>::push(const unsigned int count){
    glEnableVertexAttribArray(this->index);
    glVertexAttribPointer(
        this->index, count,
        GL_FLOAT, GL_FALSE,
        sizeof(T)*this->vertex_buffer->get_vertex_size(),
        (void*)(this->used_element_in_vertex*sizeof(T))
    );
    this->used_element_in_vertex += count;
    this->index++;
}

template class VertexArrayBuffer<float>;
