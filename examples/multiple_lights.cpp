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

void check_inputs(Window &window, Camera &camera);

int main(void){
    srand(time(0));
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Multiple Lights");
    window.set_hints();
    window.use();
    window.set_vsync(1);
    init_glew();

    Renderer renderer;
    renderer.camera.look_at(vec3(0.0f, 0.0f, -8.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));

    // creating shaders and textures
    auto obj_sp = ShaderProgram("shaders/multiple_lights.vs", "shaders/multiple_lights.fs");
    auto light_sp = ShaderProgram("shaders/multiple_lights.vs", "shaders/basic.fs");
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
    auto box_model_without_texture = Model(Mesh(vertices));
    Light light(Light::Type::POINT, vec3(0.0f, 0.0f, 0.05f), vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, 0.0f, 1.0f));
    light.scale(vec3(0.2f));
    light.move(vec3(0.0f));
    light.draw_as(box_model_without_texture);
    light.set_color(vec3(0.0f, 0.0f, 0.5f));
    renderer.add_object(light, light_sp);

    Light light2(Light::Type::POINT, vec3(0.0f, 0.05f, 0.0f), vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    light2.scale(vec3(0.2f));
    light2.move(vec3(-4.0f, -4.0f, 1.0f));
    light2.draw_as(box_model_without_texture);
    light2.set_color(vec3(0.0f, 0.5f, 0.0f));
    renderer.add_object(light2, light_sp);

    Light light3(Light::Type::DIRECTIONAL, vec3(0.1f), vec3(0.3f), vec3(0.7f));
    light3.set_direction(vec3(-1.0f));
    renderer.add_object(light3, light_sp);

    Light light4(Light::Type::SPOT, vec3(0.05f, 0.0f, 0.0f), vec3(0.5f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f));
    renderer.add_object(light4, light_sp);


    // main event loop and draw whatever we want to draw
    while(!window.should_close()){
        check_inputs(window, renderer.camera);
        renderer.clear_color(vec4(0.15, 0.15, 0.15, 1.0));
        renderer.clear_depth();

        for(int i=0;i<CUBE_COUNT;i++){
            float time = (float)glfwGetTime();
            boxes[i].move(
                boxes_positions[i] - vec3(0.0f, 0.0f, 2.0f*sinf(time))
            );
            boxes[i].rotate((time+(float)i)*10.0, vec3(1.0, 1.0, 1.0));
        }
        obj_sp.sv<vec3>("u_camera_pos", renderer.camera.get_position());
        light4.move(renderer.camera.get_position());
        light4.set_direction(renderer.camera.get_direction());

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
