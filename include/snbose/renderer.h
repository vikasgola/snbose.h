#pragma once

#include<advmath/advmath.h>
#include<snbose/object.h>
#include<snbose/shader.h>
#include<snbose/camera.h>
#include<snbose/light.h>
#include<vector>

class Renderer{
    private:
        std::vector<std::pair<Object*, ShaderProgram*>> objects;
        std::vector<std::pair<Light*, ShaderProgram*>> lights;
        static float draw_time;
    public:
        Camera camera;
        float FPS = 0.0f;
        Renderer();
        ~Renderer();
        inline float get_time(){return this->draw_time;}
        void clear_color(const vec4 color);
        void clear_depth();
        void add_object(Object &object, ShaderProgram &shader_program);
        void add_object(Light &light, ShaderProgram &shader_program);
        void draw();
};
