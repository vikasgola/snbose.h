#pragma once
#include<vector>

class IndexBuffer{
    private:
        unsigned int id;
        std::vector<unsigned int> indices;
    public:
        IndexBuffer(std::vector<unsigned int> indices);
        ~IndexBuffer();
        void bind();
        void unbind();

        inline unsigned int get_count(){return this->indices.size();}
};
