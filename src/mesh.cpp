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
    std::vector<Texture> textures
): Mesh(vertices, indices){
    this->textures = textures;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures): Mesh(vertices){
    this->textures = textures;
}

Mesh& Mesh::operator=(const Mesh &mesh){
    this->vertices = mesh.vertices;
    this->indices = mesh.indices;
    this->textures = mesh.textures;
    this->in_gpu = mesh.in_gpu;
    this->vertex_buffer = mesh.vertex_buffer;
    this->vertex_array_buffer = mesh.vertex_array_buffer;
    this->index_buffer = mesh.index_buffer;
    this->sv_vec2 = mesh.sv_vec2;
    this->sv_vec3 = mesh.sv_vec3;
    this->sv_vec4 = mesh.sv_vec4;
    return *this;
}


void Mesh::sv(std::string name, vec2 value){
    this->sv_vec2[name] = value;
}
void Mesh::sv(std::string name, vec3 value){
    this->sv_vec3[name] = value;
}
void Mesh::sv(std::string name, vec4 value){
    this->sv_vec4[name] = value;
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
    // for(auto var: this->sv_vec2){
    //     shader_program.set_uniformf<2>(var.first, var.second);
    // }
    // for(auto var: this->sv_vec3){
    //     shader_program.set_uniformf<3>(var.first, var.second);
    // }
    // for(auto var: this->sv_vec4){
    //     shader_program.set_uniformf<4>(var.first, var.second);
    // }

    std::unordered_map<std::string, int> texture_types_used;
    for(auto &texture: textures){
        auto t_type = texture.get_type();
        if(texture_types_used.find(t_type) == texture_types_used.end()) texture_types_used[t_type] = 1;
        else texture_types_used[t_type]++;

        auto name = "u_texture_" + t_type + std::to_string(texture_types_used[t_type]);
        texture.bind();
        shader_program.sv<unsigned>(name, texture.get_index());
    }

    this->bind();
    shader_program.bind();
    if(this->index_buffer != nullptr){
        glDrawElements(GL_TRIANGLES, this->index_buffer->get_count(), GL_UNSIGNED_INT, nullptr);
    }else{
        glDrawArrays(GL_TRIANGLES, 0, this->vertex_buffer->get_count());
    }
    shader_program.unbind();
    this->unbind();

    // check for any error in OpenGL
    glCheckError();
}
