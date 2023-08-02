#pragma once
#include "object.h"
#include "shader.h"
#include "2dphysics/src/vector.h"
#include "2dphysics/src/matrix.h"
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
    public:
        float FPS = 0.0f;
        Renderer();
        ~Renderer();
        void set_camera(const vec3 view);
        void use_pprojection(float fov, float aspect_ratio, float near, float far);
        void use_oprojection(float left, float right, float bottom, float top, float near, float far);
        void add_object(Object<float> &object, ShaderProgram &shader_program);
        void draw();
};
