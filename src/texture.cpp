#define STB_IMAGE_IMPLEMENTATION

#include<snbose/texture.h>
#include<stb/stb_image.h>
#include<iostream>
#include<GL/glew.h>

Texture::Texture(const std::string& file_path){
    glGenTextures(1, &this->id);
    this->data = stbi_load(file_path.c_str(), &this->width, &this->height, &this->channels, 0);
    if(!this->data){
        std::cerr<<"ERROR: failed to load file "<<file_path<<std::endl;
    }
    this->index = this->used_index++;
    this->load();
}

Texture::~Texture(){
    stbi_image_free(this->data);
    glDeleteTextures(1, &this->id);
}

void Texture::load(){
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    this->unbind();
}

void Texture::bind(){
    glActiveTexture(GL_TEXTURE0 + this->index);
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::unbind(){
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

unsigned int Texture::used_index = 0;
