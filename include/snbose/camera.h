#pragma once
#include<advmath/advmath.h>

class Camera{
    private:
        vec3 position;
        vec3 up;
        vec3 direction;
        vec3 right;
        mat4 view_matrix;
        mat4 projection;
        float pitch = 0.0f;
        float yaw = 90.0f;
        void update();
    public:
        Camera();
        void look_at(vec3 position, vec3 direction, vec3 up);
        void set_position(vec3 position);
        void set_up(vec3 up);
        void set_yaw(float yaw);
        void set_pitch(float pitch);
        void set_perspective(float fov, float aspect_ratio, float near, float far);
        void set_orthographic(float left, float right, float bottom, float top, float near, float far);

        inline float get_yaw(){ return this->yaw;}
        inline float get_pitch(){ return this->pitch;}
        inline mat4 get_view_matrix(){ return this->view_matrix;}
        inline mat4 get_projection_matrix(){ return this->projection;}
        inline vec3 get_position(){ return this->position;}
        inline vec3 get_direction(){ return this->direction;}
        inline vec3 get_right(){ return this->right;}
};
