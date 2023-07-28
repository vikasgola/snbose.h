#pragma once

class IndexBuffer{
    private:
        unsigned int id;
        const unsigned int *data;
        const unsigned int count;
    public:
        IndexBuffer(const unsigned int *data, const unsigned int count);
        ~IndexBuffer();
        void bind();
        void unbind();
};
