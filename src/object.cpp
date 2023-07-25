#include "object.h"
#include <glm/gtc/matrix_transform.hpp>

template <typename T>
Object<T>::Object(
    const T *data, const unsigned int *layout,
    const unsigned int layout_size,
    const unsigned int vertices_count
): layout(layout), layout_size(layout_size), vertex_size(0){
    for(int i=0;i<layout_size;i++){
        this->vertex_size += layout[i];
    }
    this->vertex_array_buffer = VertexArrayBuffer<T>();
    this->vertex_buffer = new VertexBuffer<T>(data, vertex_size, vertices_count);

    this->vertex_array_buffer.bind(*this->vertex_buffer);
    for(int i=0;i<layout_size;i++){
        this->vertex_array_buffer.push(layout[i]);
    }
    this->vertex_array_buffer.unbind();
}

template <typename T>
Object<T>::~Object(){
}

template <typename T>
void Object<T>::bind()
{
    this->vertex_array_buffer.rebind();

    if(this->index_buffer != NULL)
        this->index_buffer->bind();

    if(this->texture != NULL)
        this->texture->bind();
}

template <typename T>
void Object<T>::unbind(){
    this->vertex_array_buffer.unbind();

    if(this->index_buffer != NULL)
        this->index_buffer->unbind();

    if(this->texture != NULL)
        this->texture->unbind();
}

template <typename T>
void Object<T>::set_indices(const unsigned int *data, const unsigned int count){
    this->index_buffer = new IndexBuffer(data, count);
}

template <typename T>
void Object<T>::set_texture(const Texture &texture){
    this->texture = (Texture *)&texture;
}

template <typename T>
void Object<T>::scale(glm::vec3 factor){
    this->scale_factor = factor;
}

template <typename T>
void Object<T>::rotate(float angle, glm::vec3 axis){
    this->rotation_angle = angle;
    this->rotation_axis = axis;
}

template <typename T>
void Object<T>::move(glm::vec3 pos){
    this->translate = pos;
}

template <typename T>
glm::mat4 Object<T>::get_model_matrix(){
    glm::mat4 trs(1.0);
    trs = glm::translate(trs, this->translate);
    trs = glm::rotate(trs, this->rotation_angle, this->rotation_axis);
    trs = glm::scale(trs, this->scale_factor);
    return trs;
}

template class Object<float>;
