#include<snbose/vertex_array.h>
#include<GL/glew.h>

VertexArrayBuffer::VertexArrayBuffer(){
    glGenVertexArrays(1, &this->id);
}

VertexArrayBuffer::~VertexArrayBuffer(){
    glDeleteBuffers(1, &this->id);
}

void VertexArrayBuffer::bind(VertexBuffer &vertex_buffer){
    this->rebind();
    this->vertex_buffer = &vertex_buffer;
    this->vertex_buffer->bind();
    this->set_pointers();
}

void VertexArrayBuffer::bind(IndexBuffer &index_buffer){
    this->rebind();
    this->index_buffer = &index_buffer;
    this->index_buffer->bind();
}

void VertexArrayBuffer::rebind(){
    glBindVertexArray(this->id);
}

void VertexArrayBuffer::unbind(){
    glBindVertexArray(GL_NONE);
    this->vertex_buffer->unbind();
    this->index_buffer->unbind();
}

void VertexArrayBuffer::set_pointers(){
    glEnableVertexAttribArray(this->index);
    glVertexAttribPointer(
        this->index, 3,
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, position)
    );
    glEnableVertexAttribArray(++this->index);
    glVertexAttribPointer(
        this->index, 3,
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, normal)
    );
    glEnableVertexAttribArray(++this->index);
    glVertexAttribPointer(
        this->index, 2,
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, texture_coords)
    );
}
