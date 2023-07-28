#include "index_buffer.h"
#include<GL/glew.h>

IndexBuffer::IndexBuffer(const unsigned int *data, const unsigned int count): data(data), count(count){
    glGenBuffers(1, &this->id);
}

IndexBuffer::~IndexBuffer(){
    glDeleteBuffers(1, &this->id);
}

void IndexBuffer::bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->count*sizeof(unsigned int), this->data, GL_STATIC_DRAW);
}

void IndexBuffer::unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
}
