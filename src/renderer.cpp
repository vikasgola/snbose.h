#include "renderer.h"
#include<glm/gtc/type_ptr.hpp>
#include<GL/glew.h>
#include "helper.h"

Renderer::Renderer(){
}

Renderer::~Renderer(){
}

void Renderer::set_camera(const glm::vec3 view){
     this->view= glm::mat4(1.0f);
     this->view = glm::translate(this->view, view);
}

void Renderer::use_pprojection(float fov, float aspect_ratio, float near, float far){
    this->projection = glm::perspective(glm::radians(45.0f), aspect_ratio, near, far);
}

void Renderer::add_object(Object<float> &object, ShaderProgram &shader_program){
    object.bind();
    shader_program.bind();
    object.unbind();
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
        shader_program->rebind();
        shader_program->set_uniform4f("u_color", object->get_color());
        shader_program->set_uniformm4f("u_model", glm::value_ptr(object->get_model_matrix()));
        shader_program->set_uniformm4f("u_view", glm::value_ptr(this->view));
        shader_program->set_uniformm4f("u_projection", glm::value_ptr(this->projection));

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

float Renderer::draw_time = -1;
