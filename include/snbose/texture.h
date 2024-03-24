#pragma once

#include<string>

class Texture{
    private:
        unsigned int id;
        std::string type;
        std::string file_path;

        int width, height;
        int channels;
        unsigned char* data;

        unsigned int index;
        static unsigned int used_index;
    public:
        Texture(const std::string& file_path, const std::string type);
        ~Texture();

        Texture& operator=(const Texture &texture);

        void bind();
        void load();
        void unbind();
        inline std::string get_path(){ return this->file_path;}
        inline const unsigned int get_index(){return this->index;}
        inline std::string get_type(){ return this->type;}
        static void reset_index();
};
