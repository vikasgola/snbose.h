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

void Renderer::add_object(Light &light, ShaderProgram &shader_program){
    this->lights.push_back({&light, &shader_program});
}


void Renderer::draw(){
    float curr_time = (float)glfwGetTime();
    if(this->draw_time == -1){
        this->draw_time = curr_time+1e3;
    }
    this->FPS = 1.0/(curr_time-this->draw_time);
    this->draw_time = curr_time;

    // TODO: draw all lights
    for(auto &light_unit: this->lights){
        auto &light = light_unit.first;
        auto &shader_program = light_unit.second;
        if(!light->model.can_draw()) continue;

        shader_program->sv<mat4>("u_view",       this->camera.get_view_matrix());
        shader_program->sv<mat4>("u_projection", this->camera.get_projection_matrix());
        light->draw(*shader_program);
    }

    for(auto &render_unit: this->objects){
        auto &object = render_unit.first;
        auto &shader_program = render_unit.second;

        // TODO: transfer all light properties to every shader of object
        std::map<Light::Type, size_t> light_counts;
        for(size_t i=0;i<this->lights.size(); i++){
            auto &light = this->lights[i].first;
            std::string shader_light;
            if(light_counts.find(light->type) == light_counts.end())
                light_counts[light->type] = 0;

            if(light->type == Light::Type::DIRECTIONAL){
                shader_light = "directional_lights["+std::to_string(light_counts[light->type])+"]";
                shader_program->sv<vec3>(shader_light+".direction", light->direction);
            }else if(light->type == Light::Type::POINT){
                shader_light = "point_lights["+std::to_string(light_counts[light->type])+"]";
                shader_program->sv<vec3>(shader_light+".position", light->get_position());
                shader_program->sv<float>(shader_light+".constant", light->constant);
                shader_program->sv<float>(shader_light+".linear", light->linear);
                shader_program->sv<float>(shader_light+".quadratic", light->quadratic);
            }else if(light->type == Light::Type::SPOT){
                shader_light = "spot_lights["+std::to_string(light_counts[light->type])+"]";
                shader_program->sv<vec3>(shader_light+".direction", light->direction);
                shader_program->sv<vec3>(shader_light+".position", light->get_position());
                shader_program->sv<float>(shader_light+".inner_cutoff", light->inner_cutoff);
                shader_program->sv<float>(shader_light+".outer_cutoff", light->outer_cutoff);
            }else{
                fprintf(stderr, "[ERROR]: Unknown light type\n");
                exit(EXIT_FAILURE);
            }
            shader_program->sv<vec3>(shader_light+".ambient", light->ambient);
            shader_program->sv<vec3>(shader_light+".diffuse", light->diffuse);
            shader_program->sv<vec3>(shader_light+".specular", light->specular);
            light_counts[light->type]++;
        }

        shader_program->sv<mat4>("u_view",       this->camera.get_view_matrix());
        shader_program->sv<mat4>("u_projection", this->camera.get_projection_matrix());
        object->draw(*shader_program);
    }
}

float Renderer::draw_time = -1;
