#include<snbose/helper.h>
#include<snbose/window.h>
#include<snbose/texture.h>
#include<snbose/camera.h>
#include<snbose/shader.h>
#include<snbose/object.h>
#include<snbose/renderer.h>

#include<iostream>
#include<random>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CAMERA_SPEED 5

const std::string object_vertex_shader = R"OVS(
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

out vec3 frag_pos;
out vec3 normal;
out vec2 tex_cords;

uniform mat4 u_model = mat4(1.0);
uniform mat4 u_view = mat4(1.0);
uniform mat4 u_projection = mat4(1.0);

void main(){
    gl_Position = u_projection*u_view*u_model*vec4(in_position, 1.0);
    frag_pos = vec3(u_model*vec4(in_position, 1.0));
    normal = mat3(transpose(inverse(u_model)))*in_normal; // normal matrix
    tex_cords = in_tex_coord;
}
)OVS";

const std::string object_fragment_shader = R"OFS(
#version 330 core

out vec4 color;

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_cords;

uniform vec3 u_camera_pos;

struct Material {
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

uniform sampler2D u_texture_diffuse1;
uniform sampler2D u_texture_specular1;

void main(){
    vec3 ambient = light.ambient*texture(u_texture_diffuse1, tex_cords).rgb;

    vec3 n_normal = normalize(normal);

    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 diffu = light.diffuse*max(dot(light_dir, n_normal), 0.0)*texture(u_texture_diffuse1, tex_cords).rgb;

    vec3 refl_dir = reflect(-light_dir, n_normal);
    vec3 view_dir = normalize(u_camera_pos - frag_pos);
    float pre_speular = pow(max(dot(view_dir, refl_dir), 0.0), material.shininess);
    vec3 specular = light.specular*(pre_speular*texture(u_texture_specular1, tex_cords).rgb);

    float distance = length(light.position - frag_pos);
    float attenuation = 1.0f / (light.constant + distance*light.linear + distance*distance*light.quadratic);

    ambient *= attenuation;
    diffu *= attenuation;
    specular *= attenuation;

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

int main(void){
    srand(time(0));
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Point Light");
    window.set_hints();
    window.use();
    window.set_vsync(1);
    init_glew();

    Renderer renderer;
    renderer.camera.look_at(vec3(0.0f, 0.0f, -8.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));

    // creating shaders and textures
    auto obj_sp = ShaderProgram::fromSource(object_vertex_shader, object_fragment_shader);
    auto light_sp = ShaderProgram::fromSource(light_vertex_shader, light_fragment_shader);
    Texture diffuse_texture("assets/container2_diffuse.png", "diffuse");
    Texture specular_texture("assets/container2_specular.png", "specular");

    const float vertices_float[] = {
        #include "../assets/box_normal_texture.h"
    };
    std::vector<Vertex> vertices(36);
    memcpy(&vertices[0], vertices_float, sizeof(vertices_float[0])*36*8);

    auto box_mesh = Mesh(vertices, {diffuse_texture, specular_texture});
    // Object box_template(box_mesh);

    const size_t CUBE_COUNT = 80;
    std::vector<Object> boxes(CUBE_COUNT, box_mesh);
    vec3 boxes_positions[CUBE_COUNT];

    for(int i=0;i<CUBE_COUNT;i++){
        vec3 p = vec3(
            6.0f*((double) rand()/RAND_MAX) - 3.0f,
            6.0f*((double) rand()/RAND_MAX) - 3.0f,
            6.0f*((double) rand()/RAND_MAX) - 3.0f
        );
        boxes_positions[i] = p;
        boxes[i].move(p);
        boxes[i].scale(vec3(0.6f));
        renderer.add_object(boxes[i], obj_sp);
    }
    obj_sp.sv<float>("material.shininess", 64.0f);

    // Light
    auto box_mesh_without_texture = Mesh(vertices);
    Object light(box_mesh_without_texture);
    light.scale(vec3(0.2f));

    obj_sp.sv<vec3>("light.ambient", vec3(0.2f));
    obj_sp.sv<vec3>("light.diffuse", vec3(0.5f));
    obj_sp.sv<vec3>("light.specular", vec3(1.0f));
    obj_sp.sv<vec3>("light.position", light.get_position());
    obj_sp.sv<float>("light.constant", 1.0f);
    obj_sp.sv<float>("light.linear", 0.09f);
    obj_sp.sv<float>("light.quadratic", 0.032f);

    renderer.add_object(light, light_sp);

    // main event loop and draw whatever we want to draw
    while(!window.should_close()){
        check_inputs(window, renderer.camera);
        renderer.clear_color(vec4(0.15, 0.15, 0.15, 1.0));
        renderer.clear_depth();

        for(int i=0;i<CUBE_COUNT;i++){
            float time = (float)glfwGetTime();
            // boxes[i].move(
            //     boxes_positions[i] - vec3(0.0f, 0.0f, 2.0f*sinf(time))
            // );
            boxes[i].rotate((time+(float)i)*10.0, vec3(1.0, 1.0, 1.0));
        }
        obj_sp.sv<vec3>("u_camera_pos", renderer.camera.get_position());

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

    float dt = window.get_dt()*CAMERA_SPEED;
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

    static float last_x = SCREEN_WIDTH/2;
    static float last_y = SCREEN_HEIGHT/2;
    auto mouse_pos = window.get_mouse_pos();
    float offset_x = last_x - mouse_pos.first;
    float offset_y = last_y - mouse_pos.second;
    last_x = mouse_pos.first;
    last_y = mouse_pos.second;
    camera.set_pitch(camera.get_pitch()+offset_y*dt);
    camera.set_yaw(camera.get_yaw()+offset_x*dt);
}
