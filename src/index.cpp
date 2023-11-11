#include<snbose/index.h>
#include<GL/glew.h>

IndexBuffer::IndexBuffer(std::vector<unsigned int> indices): indices(indices){
    glGenBuffers(1, &this->id);
}

IndexBuffer::~IndexBuffer(){
    glDeleteBuffers(1, &this->id);
}

void IndexBuffer::bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->get_count()*sizeof(indices[0]), &this->indices[0], GL_STATIC_DRAW);
}

void IndexBuffer::unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
}
