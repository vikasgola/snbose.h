#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include<snbose/model.h>

void Model::draw(ShaderProgram &shader_program){
    for(auto &mesh: meshes){
        mesh.draw(shader_program);
    }
}

Model::Model(){}

Model::Model(Mesh mesh){
    this->add_mesh(mesh);
}

Model::Model(std::vector<Mesh> meshes){
    this->meshes = meshes;
}


Model::Model(std::string file_path){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        fprintf(stderr, "[ERROR]: %s\n", importer.GetErrorString());
        return;
    }
    this->model_path = file_path;
    this->process_scene_nodes(scene->mRootNode, scene);
}


void Model::process_scene_nodes(aiNode *node, const aiScene *scene){
    for(size_t i=0;i<node->mNumMeshes;i++){
        auto mesh = process_scene_mesh(scene->mMeshes[node->mMeshes[i]], scene);
        this->add_mesh(mesh);
    }

    for(size_t i=0;i<node->mNumChildren;i++){
        this->process_scene_nodes(node->mChildren[i], scene);
    }
}

Mesh Model::process_scene_mesh(aiMesh *mesh, const aiScene *scene){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    for(size_t i=0;i<mesh->mNumVertices;i++){
        Vertex vertex;
        vertex.position = vec3(
            mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z
        );

        auto normal = mesh->mNormals[i];
        vertex.normal = vec3(normal.x, normal.y, normal.z);

        if(mesh->mTextureCoords[0]){
            auto tex_coord = mesh->mTextureCoords[0][i];
            vertex.texture_coords = vec2(tex_coord.x, tex_coord.y);
        };

        vertices.push_back(vertex);
    }

    for(size_t i=0;i<mesh->mNumFaces;i++){
        auto face = mesh->mFaces[i];
        for(size_t i=0;i<face.mNumIndices;i++){
            indices.push_back(face.mIndices[i]);
        }
    }

    auto material = scene->mMaterials[mesh->mMaterialIndex];
    process_scene_textures(textures, material, aiTextureType_DIFFUSE);
    process_scene_textures(textures, material, aiTextureType_SPECULAR);

    return Mesh(vertices, indices, textures);
}

void Model::process_scene_textures(std::vector<Texture> &textures, aiMaterial *mat, aiTextureType type){
    Texture::reset_index();
    for(size_t i=0;i<mat->GetTextureCount(type);i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        auto dir = this->model_path.substr(0, this->model_path.find_last_of('/'));
        auto texture_path = dir+"/"+std::string(str.C_Str());

        auto it = std::find_if(textures.begin(), textures.end(), [texture_path](Texture t){return t.get_path() == texture_path;});
        if(it != textures.end()){
            textures.push_back(*it);
            continue;
        }
        std::string texture_type = aiTextureTypeToString(type);
        auto texture = Texture(texture_path, texture_type);
        textures.push_back(texture);
    }
}
