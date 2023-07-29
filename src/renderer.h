#pragma once
#include "object.h"
#include "shader.h"
#include<vector>
#include<glm/glm.hpp>

class Renderer{
    private:
        std::vector<std::pair<Object<float>*, ShaderProgram*>> objects;
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        float fov;
        float near, far;
        float aspect_ratio;
    public:
        Renderer();
        ~Renderer();
        void set_camera(const glm::vec3 view);
        void use_pprojection(float fov, float aspect_ratio, float near, float far);
        void add_object(Object<float> &object, ShaderProgram &shader_program);
        void draw();
};