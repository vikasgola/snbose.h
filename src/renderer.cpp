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
}

void Renderer::add_light(Light<float> &light, ShaderProgram &shader_program){
    this->light = &light;
    this->light_sp = &shader_program;
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

    if(this->light != nullptr){
        this->light->bind();
        this->light_sp->use();
        this->light_sp->set_uniformf<3>("u_color", this->light->get_ambient());
        this->light_sp->set_uniformm4f("u_model", this->light->get_model_matrix());
        this->light_sp->set_uniformm4f("u_view", this->camera->get_view_matrix());
        this->light_sp->set_uniformm4f("u_projection", this->camera->get_projection_matrix());
        if(this->light->have_indices()){
            glDrawElements(GL_TRIANGLES, this->light->get_indices_count(), GL_UNSIGNED_INT, nullptr);
        }else{
            glDrawArrays(GL_TRIANGLES, 0, this->light->get_vertices_count());
        }
        this->light->unbind();
    }

    for(auto &render_unit: this->objects){
        auto object = render_unit.first;
        auto shader_program = render_unit.second;
        auto material = object->get_material();

        object->bind();
        shader_program->use();
        if(shader_program->get_location("u_color") != -1){
            shader_program->set_uniformf<3>("u_color", material.ambient);
        }else{
            shader_program->set_uniformf<3>("material.ambient", material.ambient);
            shader_program->set_uniformf<3>("material.diffuse", material.diffuse);
            shader_program->set_uniformf<3>("material.specular", material.specular);
            shader_program->set_uniform1f("material.shininess", material.shininess);
        }

        shader_program->set_uniformm4f("u_model", object->get_model_matrix());
        shader_program->set_uniformm4f("u_view", this->camera->get_view_matrix());
        shader_program->set_uniformm4f("u_projection", this->camera->get_projection_matrix());

        if(this->light != nullptr){
            shader_program->set_uniformf<3>("light.ambient", this->light->get_ambient());
            shader_program->set_uniformf<3>("light.diffuse", this->light->get_diffuse());
            shader_program->set_uniformf<3>("light.specular", this->light->get_specular());
            shader_program->set_uniformf<3>("light.position", this->light->get_position());
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
