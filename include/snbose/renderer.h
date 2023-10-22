#pragma once

#include<snbose/object.h>
#include<snbose/shader.h>
#include<advmath/advmath.h>
#include<snbose/camera.h>
#include<vector>

class Renderer{
    private:
        std::vector<std::pair<Object<float>*, ShaderProgram*>> objects;
        mat4 projection = mat4(1.0f);
        mat4 view = mat4(1.0f);
        float fov;
        float near, far;
        float aspect_ratio;
        static float draw_time;
        Camera *camera;
    public:
        float FPS = 0.0f;
        Renderer();
        ~Renderer();
        inline float get_time(){return this->draw_time;}
        void clear_color(const vec4 color);
        void clear_buffer(int buffer);
        void set_camera(Camera &cam);
        void use_pprojection(float fov, float aspect_ratio, float near, float far);
        void use_oprojection(float left, float right, float bottom, float top, float near, float far);
        void add_object(Object<float> &object, ShaderProgram &shader_program);
        void draw();
};
