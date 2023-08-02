#include "renderer.h"
#include "2dphysics/src/vector.h"
#include "2dphysics/src/matrix.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>

Renderer::Renderer(){
}

Renderer::~Renderer(){
}

void Renderer::set_camera(const vec3 view){
     this->view = translation(view);
}

void Renderer::use_pprojection(float fov, float aspect_ratio, float near, float far){
    this->projection = perspective(fov, aspect_ratio, near, far);
}

void Renderer::add_object(Object<float> &object, ShaderProgram &shader_program){
    this->objects.push_back({&object, &shader_program});
}

void Renderer::draw(){
    float curr_time = (float)glfwGetTime();
    if(this->draw_time == -1){
        this->draw_time = curr_time+1e3;
    }
    this->FPS = 1.0/(curr_time-this->draw_time);
    this->draw_time = curr_time;

    for(auto &render_unit: this->objects){
        auto object = render_unit.first;
        auto shader_program = render_unit.second;

        object->bind();
        shader_program->use();
        shader_program->set_uniform4f("u_color", object->get_color());
        shader_program->set_uniformm4f("u_model", object->get_model_matrix());
        shader_program->set_uniformm4f("u_view", this->view);
        shader_program->set_uniformm4f("u_projection", this->projection);

        if(object->have_texture())
            shader_program->set_uniform1i("u_texture1", object->get_texture());

        if(object->have_indices()){
            glDrawElements(GL_TRIANGLES, object->get_indices_count(), GL_UNSIGNED_INT, nullptr);
        }else{
            glDrawArrays(GL_TRIANGLES, 0, object->get_vertices_count());
        }

        object->unbind();
    }
}

void Renderer::use_oprojection(float left, float right, float bottom, float top, float near, float far){
    this->projection = ortho(left, right, bottom, top, near, far);
}

float Renderer::draw_time = -1;
