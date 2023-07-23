#pragma once

#include<string>

class Texture{
    private:
        unsigned int id;
        int width, height;
        int channels;
        unsigned char* data;
        unsigned int index;
        static unsigned int used_index;
    public:
        Texture(const std::string& file_path);
        ~Texture();
        void bind();
        void rebind();
        void unbind();
        inline const unsigned int get_index(){return this->index;}
};
