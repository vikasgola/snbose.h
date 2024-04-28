#include <ft2build.h>
#include FT_FREETYPE_H

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

void check_inputs(Window &window, Camera &camera);

int main(void){
    srand(time(0));
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera movements");
    window.set_hints();
    window.use();
    window.set_vsync(1);
    init_glew();

    // creating shaders and textures
    ShaderProgram shader_program("shaders/3dbox.vs", "shaders/3dbox.fs");
    Texture texture("assets/container.jpg", "color");

    Renderer renderer;

    renderer.camera.look_at(vec3(0.0f, 0.0f, -8.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));


    const float vertices_float[] = {
        #include "../assets/box_normal_texture.h"
    };
    std::vector<Vertex> vertices(36);
    memcpy(&vertices[0], vertices_float, sizeof(vertices_float[0])*36*8);

    auto box_mesh = Mesh(vertices, {texture});
    Object box_template(box_mesh);

    const size_t CUBE_COUNT = 80;
    std::vector<Object> boxes(CUBE_COUNT, box_template);
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
        renderer.add_object(boxes[i], shader_program);
    }


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
