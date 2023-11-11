#include<snbose/vertex.h>
#include<GL/glew.h>

VertexBuffer::VertexBuffer(std::vector<Vertex> vertices): vertices(vertices){
    glGenBuffers(1, &this->id);
}

VertexBuffer::~VertexBuffer(){
    glDeleteBuffers(1, &this->id);
}

void VertexBuffer::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, this->get_count()*sizeof(this->vertices[0]), &this->vertices[0], GL_STATIC_DRAW);
}

void VertexBuffer::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}
