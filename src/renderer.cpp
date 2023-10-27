#include<snbose/renderer.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

Renderer::Renderer(){
}

Renderer::~Renderer(){
}

void Renderer::clear_color(const vec4 color){
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clear_depth(){
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::set_camera(Camera &cam){
     this->camera = &cam;
}

void Renderer::add_object(Object<float> &object, ShaderProgram &shader_program){
    this->objects.push_back({&object, &shader_program});
    if(object.get_type() == Object<float>::TYPE::LIGHT){
        this->light = &object;
    }
}

void Renderer::draw(){
    float curr_time = (float)glfwGetTime();
    if(this->draw_time == -1){
        this->draw_time = curr_time+1e3;
    }
    this->FPS = 1.0/(curr_time-this->draw_time);
    this->draw_time = curr_time;

    // add warning
    if(this->camera == nullptr) return;

    for(auto &render_unit: this->objects){
        auto object = render_unit.first;
        auto shader_program = render_unit.second;

        object->bind();
        shader_program->use();
        shader_program->set_uniformf<4>("u_color", object->get_color());
        shader_program->set_uniformm4f("u_model", object->get_model_matrix());
        shader_program->set_uniformm4f("u_view", this->camera->get_view_matrix());
        shader_program->set_uniformm4f("u_projection", this->camera->get_projection_matrix());

        if(object->get_type() != Object<float>::TYPE::LIGHT && this->light != nullptr){
            shader_program->set_uniformf<4>("u_light_color", this->light->get_color());
            shader_program->set_uniformf<3>("u_light_pos", this->light->get_position());
            shader_program->set_uniformf<3>("u_camera_pos", this->camera->get_position());
        }

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
