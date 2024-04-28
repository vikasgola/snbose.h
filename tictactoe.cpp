#include "src/helper.h"
#include "src/shader.h"
#include "src/texture.h"
#include "src/object.h"
#include "src/renderer.h"
#include "src/window.h"
#include "2dphysics/src/primitive.h"
#include "2dphysics/src/world.h"

#include<iostream>
using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(void){
    Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe!!!");
    if(!window.is_ready()){
        exit(EXIT_FAILURE);
    }
    window.use();
    window.set_vsync(0);
    init_glew();

    ShaderProgram sp("shaders/basic.vs", "shaders/basic.fs");
    Renderer renderer;
    renderer.use_oprojection(0, 800, 0, 600, 0.0, 1.0);

    World world(vec2(0.0, -9.8*2));
    Circle circle(10);

    Object<float> o(
        circle.get_vertex(), circle.get_vertex_layout(),
        1, circle.get_vertex_count()
    );
    vector<Object<float>> objects(2, o);
    for(int i=0;i<objects.size();i++){
        Body* b = new Body(vec2(100*(i+1), 600), vec2(20*(objects.size()+1-i), 0));
        b->set_mass(1.0);
        b->set_shape(&circle);
        world.add_body(b);

        objects[i].set_model_matrix(b->get_transform());
        renderer.add_object(objects[i], sp);
    }


    // main event loop and draw whatever we want to draw
    while(!window.should_close()){
        glClearColor(0.05, 0.05, 0.05, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.draw();
        world.step(window.get_dt());

        window.update();
    }
    return EXIT_SUCCESS;
}
