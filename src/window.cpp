#include<snbose/window.h>

Window::Window(int width, int height, const char* title): width(width), height(height), title(title){
    // Initialize GLFW
    if(glfwInit() != GLFW_TRUE){
        std::cerr<<"[ERROR]: failed to initialize glfw."<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout<<"[INFO]: GLFW successfully initialized!"<<std::endl;

    this->set_hints();

    // Create a GLFW window
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cerr<<"[ERROR]: failed to create a window."<<std::endl;
        return;
    }

    // Set the window resize callback
    glfwSetWindowSizeCallback(window, this->resize_callback);
    this->ready = true;
    this->ctime = (float)glfwGetTime();
}

Window::~Window() {
    // Destroy the GLFW window
    glfwDestroyWindow(window);

    // Terminate GLFW
    glfwTerminate();
}

void Window::use(){
    // Set the current context to the newly created window
    glfwMakeContextCurrent(window);
}

bool Window::is_ready(){
    return this->ready;
}

bool Window::should_close(){
    return glfwWindowShouldClose(window);
}

void Window::update(){
    glfwSwapBuffers(window);
    glfwPollEvents();
    float t = (float)glfwGetTime();
    this->dt = t - this->ctime;
    this->ctime = t;
}

void Window::set_hints(){
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


// This function sets the swap interval for the current OpenGL or OpenGL ES context, i.e.
// the number of screen updates to wait from the time glfwSwapBuffers was called before swapping the buffers and returning.
void Window::set_vsync(int interval){
    glfwSwapInterval(interval);
}

float Window::get_dt(){
    return this->dt;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Window::resize_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    // glViewport(0, 0, width, height);

    // warning: 'glViewport' is deprecated: first deprecated in macOS 10.14 - OpenGL API deprecated.
}
