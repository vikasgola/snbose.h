#define STB_IMAGE_IMPLEMENTATION

#include<snbose/texture.h>
#include<stb/stb_image.h>
#include<GL/glew.h>

Texture::Texture(const std::string& file_path, const std::string type){
    glGenTextures(1, &this->id);
    this->data = stbi_load(file_path.c_str(), &this->width, &this->height, &this->channels, 0);
    if(!this->data){
        fprintf(stderr, "ERROR: failed to load file %s\n", file_path.c_str());
    }
    this->index = this->used_index++;
    this->file_path = file_path;
    this->type = type;
    this->load();
}

Texture::~Texture(){
    // Copying also deteling textures
    // stbi_image_free(this->data);
    // glDeleteTextures(1, &this->id);
}

Texture& Texture::operator=(const Texture &texture){
    this->id = texture.id;
    this->type = texture.type;
    this->file_path = texture.file_path;
    this->width = texture.width;
    this->height = texture.height;
    this->channels = texture.channels;
    this->data = texture.data;
    this->index = texture.index;
    return *this;
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

void Texture::reset_index(){
    Texture::used_index = 0;
}

unsigned int Texture::used_index = 0;
