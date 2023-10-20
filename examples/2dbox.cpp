#include<snbose/helper.h>
#include<snbose/shader.h>
#include<snbose/texture.h>
#include<snbose/object.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<random>

using namespace std;

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
    ShaderProgram shader_program("shaders/advance.vs", "shaders/advance.fs");
    Texture texture("assets/container.jpg");

    const float vertices[] = {
        // p coordinates | t coordinates
        -0.5, -0.5, 0.0,    0.0, 0.0,
         0.5, -0.5, 0.0,    1.0, 0.0,
        -0.5,  0.5, 0.0,    0.0, 1.0,
         0.5,  0.5, 0.0,    1.0, 1.0,
    };
    unsigned int vertex_layout[] = {3, 2};
    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,
    };

    Object<float> wall(vertices, vertex_layout, 2, 4);
    wall.set_indices(indices, 6);
    wall.set_texture(texture);

    const size_t walls_count = 2;
    Object<float> walls[walls_count];
    for(int i=0;i<walls_count;i++){
        walls[i] = wall;
    }

    // main event loop and draw whatever we want to draw
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.05, 0.05, 0.05, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        float time = (float)glfwGetTime();
        float si = fabsf(sinf(time)*0.4f);

        // animate the wall
        for(int i=0;i<walls_count;i++){
            walls[i].scale(vec3(si, si, 1.0));
            walls[i].rotate(((float)i+1.0f+time)*10.0, vec3(0.0, 0.0, 1.0));
            walls[i].move(vec3(sinf(time+i*10), cosf(time+i*10), 0.0));
        }
        shader_program.use();

        // bind object, texture, and shader
        walls[0].bind();
        shader_program.set_uniformm4f("u_model", walls[0].get_model_matrix());
        shader_program.set_uniform1i("u_texture1", texture.get_index());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        walls[1].bind();
        shader_program.set_uniformm4f("u_model", walls[1].get_model_matrix());
        shader_program.set_uniform1i("u_texture1", texture.get_index());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}