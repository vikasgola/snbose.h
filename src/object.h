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
        IndexBuffer *index_buffer;
        Texture *texture;
        unsigned int vertex_size;
        const unsigned int layout_size;
        const unsigned int *layout;

        glm::vec3 scale_factor = glm::vec3(1.0);
        glm::vec3 translate = glm::vec3(0.0);
        glm::vec3 rotation_axis = glm::vec3(0.0, 0.0, 1.0);
        float rotation_angle = 0.0;
    public:
        Object(const T *data, const unsigned int *layout, const unsigned int layout_size, const unsigned int vertices_count);
        ~Object();
        void set_indices(const unsigned int *data, const unsigned int count);
        void set_texture(const Texture &texture);
        void scale(glm::vec3 factor);
        void rotate(float angle, glm::vec3 axis);
        void move(glm::vec3 pos);

        glm::mat4 get_model_matrix();

        void bind();
        void unbind();
};
