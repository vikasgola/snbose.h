#include "object.h"

template <typename T>
Object<T>::Object(const Object<T> &object): layout_size(object.layout_size), layout(object.layout){
    this->vertex_array_buffer = object.vertex_array_buffer;
    this->vertex_buffer = object.vertex_buffer;
    this->index_buffer = object.index_buffer;
    this->texture = object.texture;
    this->vertex_size = object.vertex_size;

    this->scale_factor = object.scale_factor;
    this->translate = object.translate;
    this->rotation_axis = object.rotation_axis;
    this->color = object.color;
    this->rotation_angle = object.rotation_angle;
}

template <typename T>
Object<T>::Object(
    const T *data, unsigned int *layout,
    unsigned int layout_size,
    const unsigned int vertices_count
) : layout(layout), layout_size(layout_size), vertex_size(0){
    for(int i=0;i<layout_size;i++){
        this->vertex_size += layout[i];
    }
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
Object<T>::Object(){
}

template <typename T>
Object<float> &Object<T>::operator=(const Object<float> &object){
    this->vertex_array_buffer = object.vertex_array_buffer;
    this->vertex_buffer = object.vertex_buffer;
    this->index_buffer = object.index_buffer;
    this->texture = object.texture;
    this->vertex_size = object.vertex_size;

    this->scale_factor = object.scale_factor;
    this->translate = object.translate;
    this->rotation_axis = object.rotation_axis;
    this->color = object.color;
    this->rotation_angle = object.rotation_angle;
    return *this;
}

template <typename T>
void Object<T>::bind(){
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
void Object<T>::scale(const vec3 factor){
    this->scale_factor = factor;
}

template <typename T>
void Object<T>::scale(const float factor_x, const float factor_y, const float factor_z){
    this->scale_factor = vec3(factor_x, factor_y, factor_z);
}

template <typename T>
void Object<T>::set_color(const vec4 color){
    this->color = color;
}

template <typename T>
void Object<T>::rotate(const float angle, const vec3 axis){
    this->rotation_angle = angle;
    this->rotation_axis = axis;
}

template <typename T>
void Object<T>::move(const vec3 pos){
    this->translate = pos;
}

template <typename T>
void Object<T>::move(const float pos_x, const float pos_y, const float pos_z){
    this->translate = vec3(pos_x, pos_y, pos_z);
}

template <typename T>
mat4 Object<T>::get_model_matrix(){
    return transform(
        this->scale_factor,
        this->rotation_axis*this->rotation_angle,
        this->translate
    );
}

template class Object<float>;
