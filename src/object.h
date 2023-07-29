#pragma once
#include "vertex_array_buffer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture.h"
#include<glm/glm.hpp>

template <typename T>
class Object{
    private:
        VertexArrayBuffer<T> vertex_array_buffer;
        VertexBuffer<T> *vertex_buffer;
        IndexBuffer *index_buffer = NULL;
        Texture *texture = NULL;
        unsigned int vertex_size;
        unsigned int layout_size;
        unsigned int *layout;

        glm::vec3 scale_factor = glm::vec3(1.0);
        glm::vec3 translate = glm::vec3(0.0);
        glm::vec3 rotation_axis = glm::vec3(0.0, 0.0, 1.0);
        glm::vec4 color = glm::vec4(1.0);
        float rotation_angle = 0.0;
    public:
        Object(const T *data, unsigned int *layout, unsigned int layout_size, const unsigned int vertices_count);
        Object(const Object<T> &object);
        ~Object();
        Object();
        Object<float>& operator=(const Object<float>& object);
        void set_indices(const unsigned int *data, const unsigned int count);
        void set_texture(const Texture &texture);
        void set_color(const glm::vec4 color);

        void scale(const glm::vec3 factor);
        void scale(const float factor_x, const float factor_y, const float factor_z);
        void rotate(const float angle, const glm::vec3 axis);
        void move(const glm::vec3 pos);
        void move(const float pos_x, const float pos_y, const float pos_z);

        inline glm::vec3 get_position(){return this->translate;}
        inline glm::vec4 get_color() {return this->color;}
        inline unsigned int get_texture(){return this->texture->get_index();}
        inline unsigned int get_indices_count(){ return this->index_buffer->get_count();}
        inline unsigned int get_vertices_count(){ return this->vertex_buffer->get_vertices_count();}


        inline bool have_indices(){ return this->index_buffer != NULL;}
        inline bool have_texture(){ return this->texture != NULL;}

        glm::mat4 get_model_matrix();

        void bind();
        void unbind();
};
