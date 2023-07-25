#include "src/helper.h"
#include "src/shader.h"
#include "src/vertex_buffer.h"
#include "src/index_buffer.h"
#include "src/vertex_array_buffer.h"
#include "src/texture.h"
#include "src/object.h"

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>
#include<random>


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
    // glfwSwapInterval(10);

    // initialize the glew
    // Needs to bind with opengl on graphic card as OS devloper are bitch
    // and they only focus on device specifc gpu api's i.e. directx, metal etc.
    if(glewInit() != GLEW_OK){
        std::cerr<<"ERROR: failed to intialize glew."<<std::endl;
        return EXIT_FAILURE;
    }
    // std::cout<<glGetString(GL_VERSION)<<std::endl;


    // creating shaders and textures
    ShaderProgram shader_program("shaders/basic.vs", "shaders/basic.fs");
    Texture texture("assets/container.jpg");

    const float vertices[] = {
        // p coordinates | t coordinates
        -0.5, -0.5,    0.0, 0.0,
         0.5, -0.5,    1.0, 0.0,
        -0.5,  0.5,    0.0, 1.0,
         0.5,  0.5,    1.0, 1.0,
    };
    const unsigned int vertex_layout[] = {2, 2};
    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,
    };

    Object<float> wall(vertices, vertex_layout, 2, 4);
    wall.set_indices(indices, 6);
    wall.set_texture(texture);

    // main event loop and draw whatever we want to draw
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.05, 0.05, 0.05, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        float time = (float)glfwGetTime();
        float si = fabsf(sinf(time)*0.4f);

        // animate the wall
        wall.scale(glm::vec3(si, si, 1.0));
        wall.rotate(time, glm::vec3(0.0, 0.0, 1.0));
        wall.move(glm::vec3(sinf(time), cosf(time), 0.0));

        // bind object, texture, and shader
        wall.bind();
        shader_program.bind();

        // shader_program.set_uniform4f("u_color", 1.0, 1.0, 1.0, 1.0);
        shader_program.set_uniform1i("texture1", texture.get_index());
        shader_program.set_uniformm4f("u_model", glm::value_ptr(wall.get_model_matrix()));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}