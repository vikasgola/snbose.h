#pragma once

#include<snbose/object.h>
#include<snbose/shader.h>
#include<advmath/advmath.h>
#include<snbose/camera.h>
#include<vector>

class Renderer{
    private:
        std::vector<std::pair<Object<float>*, ShaderProgram*>> objects;
        static float draw_time;
        Camera *camera = nullptr;
        Object<float> *light = nullptr;
    public:
        float FPS = 0.0f;
        Renderer();
        ~Renderer();
        inline float get_time(){return this->draw_time;}
        void clear_color(const vec4 color);
        void clear_depth();
        void set_camera(Camera &cam);
        void add_object(Object<float> &object, ShaderProgram &shader_program);
        void draw();
};
