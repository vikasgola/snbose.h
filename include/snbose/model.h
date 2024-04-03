#pragma once
#include <assimp/scene.h>

#include<vector>
#include<snbose/mesh.h>
#include<snbose/shader.h>
#include<string>

class Model{
    private:
        std::string model_path;
        std::vector<Mesh> meshes;
        void process_scene_nodes(aiNode *node, const aiScene *scene);
        Mesh process_scene_mesh(aiMesh *mesh, const aiScene *scene);
        void process_scene_textures(std::vector<Texture> &textures, aiMaterial *mat, aiTextureType type);
    public:
        Model();
        Model(Mesh meshes);
        Model(std::vector<Mesh> meshes);
        Model(std::string file_path);

        inline bool can_draw(){return this->meshes.size() > 0;}
        void draw(ShaderProgram &shader_program);
        inline std::vector<Mesh> get_meshes(){return this->meshes;}
        inline void add_mesh(Mesh mesh){ this->meshes.push_back(mesh);}
};
