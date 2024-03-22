#include<snbose/helper.h>
#include<snbose/window.h>
#include<snbose/camera.h>
#include<snbose/renderer.h>
#include<snbose/object.h>
#include<snbose/shader.h>
#include<snbose/material.h>
#include<cmath>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const std::string object_vertex_shader = R"OVS(
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

out vec3 frag_pos;
out vec3 normal;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view = mat4(1.0);
uniform mat4 u_projection = mat4(1.0);

void main(){
    gl_Position = u_projection*u_view*u_model*vec4(in_position, 1.0);
    frag_pos = vec3(u_model*vec4(in_position, 1.0));
    normal = mat3(transpose(inverse(u_model)))*in_normal; // normal matrix
}
)OVS";

const std::string object_fragment_shader = R"OFS(
#version 330 core

out vec4 color;

in vec3 normal;
in vec3 frag_pos;

uniform vec3 u_camera_pos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main(){
    vec3 ambient = light.ambient*material.ambient;

    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 diffu = light.diffuse*(max(dot(light_dir, normal), 0.0)*material.diffuse);

    vec3 refl_dir = reflect(-light_dir, normal);
    vec3 view_dir = normalize(u_camera_pos - frag_pos);
    float pre_speular = pow(max(dot(view_dir, refl_dir), 0.0), material.shininess);
    vec3 specular = light.specular*(pre_speular*material.specular);

    color = vec4(ambient+diffu+specular, 1.0);
}
)OFS";

const std::string light_vertex_shader = object_vertex_shader;

const std::string light_fragment_shader = R"OFS(
#version 330 core

out vec4 color;

uniform vec3 u_color = vec3(1.0);

void main(){
    color = vec4(u_color, 1.0);
}
)OFS";

void check_inputs(Window &window, Camera &camera);

int main(){
    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Lighting");
    window.set_hints();
    window.use();
    init_glew();


    Renderer renderer;
    renderer.camera.look_at(vec3(0.0f, 8.0f, -12.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));
    renderer.camera.set_pitch(renderer.camera.get_pitch()-20.0f);

    auto obj_sp = ShaderProgram::fromSource(object_vertex_shader, object_fragment_shader);
    auto light_sp = ShaderProgram::fromSource(light_vertex_shader, light_fragment_shader);

    const size_t vertex_size = 6;
    const float cube_vertices[] = {
        #include "../assets/box_normal.h"
    };
    std::vector<Vertex> vertices;
    for(size_t i=0;i<36;i++)
        vertices.push_back({
            .position = vec3(cube_vertices[i*vertex_size], cube_vertices[i*vertex_size+1], cube_vertices[i*vertex_size+2]),
            .normal = vec3(cube_vertices[i*vertex_size+3], cube_vertices[i*vertex_size+3+1], cube_vertices[i*vertex_size+3+2])
        });

    Mesh box_mesh(vertices);

    Object floor(box_mesh);
    floor.set_material(MATERIAL_GOLD);
    floor.scale(8.0f, 1.0f, 8.0f);

    Object light(box_mesh);
    light.move(0.0f, 2.0f, 0.0f);

    std::vector<Object> boxes(2, box_mesh);
    boxes[0].move(vec3(2.0f, 1.0f, 2.0f));
    boxes[1].move(vec3(-2.0f, 1.0f, -2.0f));
    boxes[0].set_material(MATERIAL_EMERALD);
    boxes[1].set_material(MATERIAL_PEARL);

    // shader properties
    obj_sp.sv("light.ambient", vec3(0.2f));
    obj_sp.sv("light.diffuse", vec3(0.5f));
    obj_sp.sv("light.specular", vec3(1.0f));

    renderer.add_object(light, light_sp);
    renderer.add_object(floor, obj_sp);
    renderer.add_object(boxes[0], obj_sp);
    renderer.add_object(boxes[1], obj_sp);

    vec3 pos = light.get_position();
    while(!window.should_close()){
        check_inputs(window, renderer.camera);
        renderer.clear_color(vec4(0.12f, 0.12f, 0.12f, 1.0f));
        renderer.clear_depth();

        light.move(pos+vec3(sinf(renderer.get_time()), 0.0f, cosf(renderer.get_time()))*5.0f);
        // floor.rotate(renderer.get_time()*2.0f, vec3(1.0, 1.0f, 1.0f));
        obj_sp.sv("u_camera_pos", renderer.camera.get_position());
        obj_sp.sv("light.position", light.get_position());

        renderer.draw();
        window.update();
    }
    return EXIT_SUCCESS;
}


void check_inputs(Window &window, Camera &camera){
    if (window.is_key_pressed(GLFW_KEY_ESCAPE)){
        window.release_mouse();
    }else if(window.is_key_pressed(GLFW_MOUSE_BUTTON_LEFT)){
        window.capture_mouse();
    }

    if(!window.is_mouse_captured()) return;

    float dt = window.get_dt();
    vec3 position = camera.get_position();

    if(window.is_key_pressed(GLFW_KEY_W)){
        position = position + camera.get_direction()*dt;
    }
    if(window.is_key_pressed(GLFW_KEY_S)){
        position = position - camera.get_direction()*dt;
    }
    if(window.is_key_pressed(GLFW_KEY_A)){
        position = position - camera.get_right()*dt;
    }
    if(window.is_key_pressed(GLFW_KEY_D)){
        position = position + camera.get_right()*dt;
    }
    camera.set_position(position);

    static float last_x = WINDOW_WIDTH/2;
    static float last_y = WINDOW_HEIGHT/2;
    auto mouse_pos = window.get_mouse_pos();
    float offset_x = last_x - mouse_pos.first;
    float offset_y = last_y - mouse_pos.second;
    last_x = mouse_pos.first;
    last_y = mouse_pos.second;
    camera.set_pitch(camera.get_pitch()+offset_y*dt);
    camera.set_yaw(camera.get_yaw()+offset_x*dt);
}
