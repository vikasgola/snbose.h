#include<snbose/helper.h>
#include<snbose/shader.h>
#include<snbose/texture.h>
#include<snbose/object.h>
#include<snbose/renderer.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<vector>
#include<random>

using namespace std;

#define WALL_COUNT 10

int main(void){
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
    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello World!", NULL, NULL);
    if(!window){
        std::cerr<<"ERROR: failed to create a window."<<std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    // set context to new created windows
    glfwMakeContextCurrent(window);

    // enable vsync. swap buffers when 0 screen update has happened
    glfwSwapInterval(0);

    // initialize the glew
    // Needs to bind with opengl on graphic card as OS devloper are bitch
    // and they only focus on device specifc gpu api's i.e. directx, metal etc.
    if(glewInit() != GLEW_OK){
        std::cerr<<"ERROR: failed to intialize glew."<<std::endl;
        return EXIT_FAILURE;
    }
    // std::cout<<glGetString(GL_VERSION)<<std::endl;


    // creating shaders and textures
    ShaderProgram shader_program("shaders/2dbox.vs", "shaders/2dbox.fs");
    Texture texture("assets/container.jpg", "color");

    std::vector<Vertex> vertices = {
        // p coordinates | t coordinates
        {
            .position = vec3(-0.5, -0.5, 0.0),
            .texture_coords = vec2(0.0, 0.0)
        },
        {
            .position = vec3(0.5, -0.5, 0.0),
            .texture_coords = vec2(1.0, 0.0)
        },
        {
            .position = vec3(-0.5,  0.5, 0.0),
            .texture_coords = vec2(0.0, 1.0)
        },
        {
            .position = vec3(0.5,  0.5, 0.0),
            .texture_coords = vec2(1.0, 1.0)
        }
    };
    std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 2, 3,
    };

    Renderer renderer;

    auto wall_mesh = Mesh(vertices, indices, {texture});

    vector<Object> walls(WALL_COUNT, wall_mesh);
    for(int i=0;i<WALL_COUNT;i++){
        renderer.add_object(walls[i], shader_program);
    }

    // main event loop and draw whatever we want to draw
    while(!glfwWindowShouldClose(window)){
        renderer.clear_color(vec4(0.05, 0.05, 0.05, 1.0));
        renderer.clear_depth();
        float time = (float)glfwGetTime();
        float si = fabsf(sinf(time)*0.4f);

        // animate the wall
        for(int i=0;i<WALL_COUNT;i++){
            walls[i].scale(vec3(si, si, 1.0));
            walls[i].rotate(((float)i+1.0f+time)*10.0, vec3(0.0, 0.0, 1.0));
            walls[i].move(vec3(sinf(time+2.0*M_PI*i/WALL_COUNT), cosf(time+(float)2.0*M_PI*i/WALL_COUNT), 0.0));
        }
        renderer.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}