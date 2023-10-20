#include<snbose/helper.h>
#include<snbose/shader.h>
#include<snbose/vertex_array_buffer.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<vector>

using namespace std;

void create_board(vector<float> &board, vector<unsigned int> &indices, vector<unsigned int> &indices2){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            float row = i*2/8.0 - 1;
            float col = j*2/8.0 - 1;
            int k = (i*9 + j)*3;
            board[k++] = row;
            board[k++] = col;
            board[k++] = 0.0;
        }
    }

    int k=0, k2=0, t = 1;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if((i*8 + j)%2 == t){
                indices[k++] = i*9 + j;
                indices[k++] =  i*9 + j+1;
                indices[k++] = (i+1)*9 + j;
                indices[k++] = i*9 + j+1;
                indices[k++] = (i+1)*9 + j;
                indices[k++] = (i+1)*9 + j+1;
            }else{
                indices2[k2++] = i*9 + j;
                indices2[k2++] =  i*9 + j+1;
                indices2[k2++] = (i+1)*9 + j;
                indices2[k2++] = i*9 + j+1;
                indices2[k2++] = (i+1)*9 + j;
                indices2[k2++] = (i+1)*9 + j+1;
            }
        }
        t = 1-t;
    }
}

int main(void){
    // initialize the glfw environ
    // helps to create window, independent of os.
    if(glfwInit() != GLFW_TRUE){
        cerr<<"ERROR: failed to initialize glfw."<<endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    GLFWwindow *window = glfwCreateWindow(800, 600, "Checker Board", NULL, NULL);
    if(!window){
        cerr<<"ERROR: failed to create a window."<<endl;
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
        cerr<<"ERROR: failed to intialize glew."<<endl;
        return EXIT_FAILURE;
    }
    // cout<<glGetString(GL_VERSION)<<endl;


    vector<float> board_vertices(9*9*3);
    vector<unsigned int> indices1(32*3*2);
    vector<unsigned int> indices2(32*3*2);
    create_board(board_vertices, indices1, indices2);

    ShaderProgram sp("shaders/checker_board.vs", "shaders/checker_board.fs");

    VertexArrayBuffer<float> vao;
    VertexBuffer<float> vbo(&board_vertices[0], 3, 9*9);
    IndexBuffer ibo1(&indices1[0], 32*3*2);
    IndexBuffer ibo2(&indices2[0], 32*3*2);


    vao.bind(vbo);
        vao.push(3);
    vao.unbind();

    // main event loop and draw whatever we want to draw
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.05, 0.05, 0.05, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        sp.use();

        vao.bind(ibo1);
        sp.set_uniform4f("a_color", vec4(0.80, 0.80, 0.80, 1.0));
        glDrawElements(GL_TRIANGLES, 32*6, GL_UNSIGNED_INT, 0);
        vao.unbind();

        vao.bind(ibo2);
        sp.set_uniform4f("a_color", vec4(0.15, 0.15, 0.15, 1.0));
        glDrawElements(GL_TRIANGLES, 32*6, GL_UNSIGNED_INT, 0);
        vao.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}