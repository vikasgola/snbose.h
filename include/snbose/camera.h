#pragma once
#include<advmath/advmath.h>

class Camera{
    private:
        vec3 position;
        vec3 up;
        vec3 direction;
        mat4 view_matrix;
        vec3 right;
        float pitch = 0.0f;
        float yaw = -90.0f;
        void update();
    public:
        Camera();
        void look_at(vec3 position, vec3 target, vec3 up);
        void set_position(vec3 position);
        void set_up(vec3 up);
        void set_yaw(float yaw);
        void set_pitch(float pitch);
        inline float get_yaw(){ return this->yaw;}
        inline float get_pitch(){ return this->pitch;}
        inline mat4 get_view_matrix(){ return this->view_matrix;}
        inline vec3 get_position(){ return this->position;}
        inline vec3 get_direction(){ return this->direction;}
        inline vec3 get_right(){ return this->right;}
};
