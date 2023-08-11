#pragma once
#include <GLFW/glfw3.h>
#include<iostream>

class Window {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        void use();
        bool is_ready();
        bool should_close();
        void update();
        void set_hints();
        void set_vsync(int interval);

    private:
        GLFWwindow* window;
        bool ready = false;
        int width;
        int height;
        const char* title;
        static void resize_callback(GLFWwindow* window, int width, int height);
};