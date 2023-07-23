#include "src/helper.h"
#include "src/shader.h"
#include "src/vertex_buffer.h"
#include "src/index_buffer.h"
#include "src/vertex_array_buffer.h"
#include "src/texture.h"

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

    float positions[] = {
        -0.5, -0.5, 0.0, 0.0,
         0.5, -0.5, 1.0, 0.0,
        -0.5,  0.5, 0.0, 1.0,
         0.5,  0.5, 1.0, 1.0,
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,
    };

    // create vertex array buffer which stores pointer to vbo and ibo for easy binding
    VertexArrayBuffer<float> vertex_array_buffer;
    VertexBuffer<float> vertex_buffer(positions, 4, 4);
    IndexBuffer index_buffer(indices, 6);
    Texture texture("assets/container.jpg");

    // read and create shaders
    Shader vertex_shader(GL_VERTEX_SHADER, "shaders/basic.vs");
    Shader fragment_shader(GL_FRAGMENT_SHADER, "shaders/basic.fs");
    ShaderProgram shader_program(vertex_shader, fragment_shader);


    // bind vbo and ibo to vao
    vertex_array_buffer.bind(vertex_buffer);
    vertex_array_buffer.bind(index_buffer);
    // enable first vertex attrib array with positions
    vertex_array_buffer.push(2);
    vertex_array_buffer.push(2);
    shader_program.bind();
    texture.bind();

    // unbind everthing
    vertex_array_buffer.unbind();
    texture.unbind();
    shader_program.unbind();

    int r = 0, g = 0, b = 0;
    int rt = rand()%5, gt = rand()%5, bt = rand()%5;
    // main event loop and draw whatever we want to draw
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.05, 0.05, 0.05, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        r = (r+rt)%256;
        g = (g+gt)%256;
        b = (b+bt)%256;

        float time = (float)glfwGetTime();
        float si = fabsf(sinf(time));
        // trs
        // i.e. scale, translate, and, rotation
        glm::mat4 trs(1.0);
        trs = glm::translate(trs, glm::vec3(
            sinf(time)*cosf(time),
            cosf(time)*sinf(time),
            0.0)
        );
        trs = glm::rotate(trs, time, glm::vec3(0.0, 0.0, 1.0));
        trs = glm::scale(trs, glm::vec3(si, si, 1.0));

        texture.rebind();
        vertex_array_buffer.rebind();

        shader_program.rebind();
        // shader_program.set_uniform4f("u_color", r/256.0, g/256.0, b/256.0, 1.0);
        shader_program.set_uniform4f("u_color", 1.0, 1.0, 1.0, 1.0);
        shader_program.set_uniform1i("texture1", texture.get_index());
        shader_program.set_uniformm4f("u_trs", glm::value_ptr(trs));

        // draw
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}