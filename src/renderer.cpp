#include<snbose/renderer.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

Renderer::Renderer(){
}

Renderer::~Renderer(){
}

void Renderer::clear_color(const vec4 color){
    glClearColor(color.x(), color.y(), color.z(), color.w());
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clear_depth(){
    glClear(GL_DEPTH_BUFFER_BIT);
}


void Renderer::add_object(Object &object, ShaderProgram &shader_program){
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
        shader_program->bind();

        shader_program->set_uniformm4f("u_view", this->camera.get_view_matrix());
        shader_program->set_uniformm4f("u_projection", this->camera.get_projection_matrix());
        object->draw(*shader_program);

        shader_program->unbind();
    }
}

float Renderer::draw_time = -1;
