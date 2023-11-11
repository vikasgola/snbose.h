#pragma once
#include<advmath/advmath.h>
#include<vector>

struct Vertex{
    vec3 position;
    vec3 normal;
    vec2 texture_coords;
};

class VertexBuffer{
    private:
        std::vector<Vertex> vertices;
        unsigned int id;
    public:
        VertexBuffer(std::vector<Vertex> vertices);
        ~VertexBuffer();
        void bind();
        void unbind();
        inline const unsigned int get_count() {return this->vertices.size();}
};
