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
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World!", NULL, NULL);
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
    ShaderProgram shader_program("shaders/basic.vs", "shaders/basic.fs");
    Texture texture("assets/container.jpg");

    const float vertices[] = {
        #include "assets/box.h"
    };
    const unsigned int vertex_layout[] = {3, 2};
    Object<float> box(vertices, vertex_layout, 2, 36);
    box.set_texture(texture);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -100.0f));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f);

    const size_t CUBE_COUNT = 15;
    glm::vec3 cube_positions[CUBE_COUNT] = {
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),

        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),

        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
        glm::vec3(4.0f*rand()/RAND_MAX - 2.0f, 4.0f*rand()/RAND_MAX - 2.0f, 90.0f + 7.0f*rand()/RAND_MAX),
    };

    // main event loop and draw whatever we want to draw
    while(!glfwWindowShouldClose(window)){
        check_inputs(window);
        glClearColor(0.05, 0.05, 0.05, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float time = (float)glfwGetTime();

        // bind object, texture, and shader
        box.bind();
        shader_program.bind();
        shader_program.set_uniform1i("texture1", texture.get_index());
        shader_program.set_uniformm4f("u_view", glm::value_ptr(view));
        shader_program.set_uniformm4f("u_projection", glm::value_ptr(projection));
        for(int i=0;i<CUBE_COUNT;i++){
            glm::vec3 t(cube_positions[i]);
            t.z = cube_positions[i].z + 2.0f*sinf(time);
            box.scale(glm::vec3(0.7));
            box.rotate(time+i, glm::vec3(1.0, 1.0, 1.0));
            box.move(t);
            shader_program.set_uniformm4f("u_model", glm::value_ptr(box.get_model_matrix()));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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