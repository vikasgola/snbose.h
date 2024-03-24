
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
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera movements");
    window.set_hints();
    window.use();
    window.set_vsync(1);
    init_glew();

    // creating shaders and textures
    ShaderProgram shader_program("shaders/model.vs", "shaders/model.fs");

    Renderer renderer;
    renderer.camera.look_at(vec3(0.0f, 0.0f, -20.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));

    Model model("./assets/bagpack/model.obj");
    const size_t BAGPACKS_COUNT = 100;
    std::vector<Object> bagpacks(BAGPACKS_COUNT, model);
    vec3 bagpack_positions[BAGPACKS_COUNT];

    for(int i=0;i<100;i++){
        vec3 p = vec3(
            100.0f*((double) rand()/RAND_MAX) - 50.0f,
            100.0f*((double) rand()/RAND_MAX) - 50.0f,
            7.0f*((double) rand()/RAND_MAX)
        );
        bagpack_positions[i] = p;
        bagpacks[i].move(p);
        bagpacks[i].scale(vec3(0.02));
        renderer.add_object(bagpacks[i], shader_program);
    }

    int counter = 0;
    // main event loop and draw whatever we want to draw
    while(!window.should_close()){
        check_inputs(window, renderer.camera);
        renderer.clear_color(vec4(0.15, 0.15, 0.15, 1.0));
        renderer.clear_depth();
        if(counter % 5 == 0)
            std::cout<<"\rFPS:"<<renderer.FPS<<std::flush;

        for(int i=0;i<BAGPACKS_COUNT;i++){
            float time = (float)glfwGetTime();
            bagpacks[i].move(
                bagpack_positions[i] - vec3(0.0f, 0.0f, 20.0f*sinf(time))
            );
            bagpacks[i].rotate((time+(float)i)*10.0, vec3(1.0, 1.0, 1.0));
        }
        renderer.draw();
        window.update();
        counter++;
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
        position = position + camera.get_direction()*dt*CAMERA_SPEED;
    }
    if(window.is_key_pressed(GLFW_KEY_S)){
        position = position - camera.get_direction()*dt*CAMERA_SPEED;
    }
    if(window.is_key_pressed(GLFW_KEY_A)){
        position = position - camera.get_right()*dt*CAMERA_SPEED;
    }
    if(window.is_key_pressed(GLFW_KEY_D)){
        position = position + camera.get_right()*dt*CAMERA_SPEED;
    }
    camera.set_position(position);

    static float last_x = SCREEN_WIDTH/2;
    static float last_y = SCREEN_HEIGHT/2;
    auto mouse_pos = window.get_mouse_pos();
    float offset_x = last_x - mouse_pos.first;
    float offset_y = last_y - mouse_pos.second;
    last_x = mouse_pos.first;
    last_y = mouse_pos.second;
    camera.set_pitch(camera.get_pitch()+offset_y*dt*CAMERA_SPEED);
    camera.set_yaw(camera.get_yaw()+offset_x*dt*CAMERA_SPEED);
}
