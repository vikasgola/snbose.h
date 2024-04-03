#include<snbose/helper.h>
#include<snbose/shader.h>
#include<snbose/object.h>
#include<snbose/texture.h>
#include<snbose/camera.h>
#include<snbose/renderer.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<random>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void check_inputs(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main(void){
    srand(time(0));
    // initialize the glfw environ
    // helps to create window, independent of os.
    if(glfwInit() != GLFW_TRUE){
        std::cerr<<"ERROR: failed to initialize glfw."<<std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3box", NULL, NULL);
    if(!window){
        std::cerr<<"ERROR: failed to create a window."<<std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    // set context to new created windows
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // enable vsync. swap buffers when 0 screen update has happened
    // glfwSwapInterval(10);

    // initialize the glew
    // Needs to bind with opengl on graphic card as OS devloper are bitch
    // and they only focus on device specifc gpu api's i.e. directx, metal etc.
    if(glewInit() != GLEW_OK){
        std::cerr<<"ERROR: failed to intialize glew."<<std::endl;
        return EXIT_FAILURE;
    }
    // std::cout<<glGetString(GL_VERSION)<<std::endl;

    glEnable(GL_DEPTH_TEST);

    // creating shaders and textures
    ShaderProgram shader_program("shaders/3dbox.vs", "shaders/3dbox.fs");
    Texture texture("assets/container.jpg", "color");

    Renderer renderer;
    renderer.camera.look_at(vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));

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
            7.0f*((double) rand()/RAND_MAX)
        );
        boxes_positions[i] = p;
        boxes[i].move(p);
        boxes[i].scale(vec3(0.4));
        renderer.add_object(boxes[i], shader_program);
    }

    // main event loop and draw whatever we want to draw
    while(!glfwWindowShouldClose(window)){
        check_inputs(window);
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void check_inputs(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}