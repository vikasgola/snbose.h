#pragma once
#include<advmath/advmath.h>
#include<snbose/model.h>
#include<snbose/shader.h>

class Object{
    private:
        Model *model;
        vec3 scale_factor = vec3(1.0f);
        vec3 translate = vec3(0.0f);
        vec3 rotation_axis = vec3(0.0f, 0.0f, 1.0f);
        float rotation_angle = 0.0;
    public:
        Object();
        Object(Mesh &mesh);
        Object(Model &model);
        Object& operator=(const Object &object);
        void draw(ShaderProgram &shader_program);

        void scale(const vec3 factor);
        void scale(const float factor_x, const float factor_y, const float factor_z);
        void rotate(const float angle, const vec3 axis);
        void move(const vec3 pos);
        void move(const float pos_x, const float pos_y, const float pos_z);

        mat4 get_model_matrix();
        inline vec3 get_position(){return this->translate;}
};
