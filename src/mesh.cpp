#include<snbose/helper.h>
#include<snbose/mesh.h>
#include<unordered_map>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices){
    this->vertices = vertices;
    this->indices = indices;
}

Mesh::Mesh(std::vector<Vertex> vertices){
    this->vertices = vertices;
}

Mesh::Mesh(
    std::vector<Vertex> vertices,
    std::vector<unsigned int> indices,
    std::vector<Texture*> textures
): Mesh(vertices, indices){
    this->textures = textures;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture*> textures): Mesh(vertices){
    this->textures = textures;
}

void Mesh::send_to_gpu(){
    this->vertex_buffer = new VertexBuffer(this->vertices);
    if(this->indices.size() > 0){
        this->index_buffer = new IndexBuffer(this->indices);
    }
    this->vertex_array_buffer.bind(*this->vertex_buffer);
    this->vertex_array_buffer.unbind();
    this->in_gpu = true;
}

void Mesh::bind(){
    if(!this->in_gpu) this->send_to_gpu();
    this->vertex_array_buffer.rebind();
    if(this->index_buffer != nullptr){
        this->index_buffer->bind();
    }
}

void Mesh::unbind(){
    this->vertex_array_buffer.unbind();
    if(this->index_buffer != nullptr){
        this->index_buffer->unbind();
    }
}

void Mesh::draw(ShaderProgram &shader_program){
    shader_program.bind();
    this->bind();

    std::unordered_map<std::string, int> texture_types_used;
    for(auto texture: textures){
        auto t_type = texture->get_type();
        if(texture_types_used.find(t_type) == texture_types_used.end()) texture_types_used[t_type] = 1;
        else texture_types_used[t_type]++;

        auto name = "u_texture_" + t_type + std::to_string(texture_types_used[t_type]);
        // fprintf(stdout, "[INFO]: used %d.\n", texture->get_index());
        texture->bind();
        shader_program.set_uniform1i(name, texture->get_index());
    }

    if(this->index_buffer != nullptr)
        glDrawElements(GL_TRIANGLES, this->index_buffer->get_count(), GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, this->vertex_buffer->get_count());

    // this->unbind();
    // shader_program.unbind();
}
