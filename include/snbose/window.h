#pragma once
#include<GLFW/glfw3.h>
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
        float get_dt();
        bool is_key_pressed(uint key);
        void close();
        void capture_mouse();
        void release_mouse();
        inline bool is_mouse_captured(){ return this->mouse_captured;}
        std::pair<double, double> get_mouse_pos();

    private:
        float ctime;
        float dt = 0;
        GLFWwindow* window;
        bool ready = false;
        int width;
        int height;
        const char* title;
        bool mouse_captured;
        static void resize_callback(GLFWwindow* window, int width, int height);
};