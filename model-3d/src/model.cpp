#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

#include "model.h"
#include "mesh.h"
#include "shader.h"

void model_create(model_t *model, const char *path, bool gamma_correction) {
    model->gamma_correction = gamma_correction;

    model_load(model, std::string(path));
}

void model_load(model_t *model, std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                  aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString()
                  << std::endl;
        return;
    }
    model->directory = path.substr(0, path.find_last_of('/'));

    model_process_node(model, scene->mRootNode, scene);
}

void model_draw(shader_program_t *shader, model_t *model) {
    for (unsigned int i = 0; i < model->meshes.size(); i++) {
        mesh_draw(shader, &model->meshes[i]);
    }
}

void model_process_node(model_t *model, aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];
        mesh_t mesh;
        model_process_mesh(model, &mesh, ai_mesh, scene);
        model->meshes.push_back(mesh);
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        model_process_node(model, node->mChildren[i], scene);
    }
}

void model_process_mesh(model_t *model, mesh_t *mesh, aiMesh *ai_mesh,
                        const aiScene *scene) {
    // data to fill
    std::vector<vertex_t> vertices;
    std::vector<unsigned int> indices;
    std::vector<texture_t> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
        vertex_t vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses
                          // its own vector class that doesn't directly convert
                          // to glm's vec3 class so we transfer the data to this
                          // placeholder glm::vec3 first.
        // positions
        vector.x = ai_mesh->mVertices[i].x;
        vector.y = ai_mesh->mVertices[i].y;
        vector.z = ai_mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (ai_mesh->HasNormals()) {
            vector.x = ai_mesh->mNormals[i].x;
            vector.y = ai_mesh->mNormals[i].y;
            vector.z = ai_mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        // does the mesh contain texture coordinates?
        if (ai_mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We
            // thus make the assumption that we won't use models where a vertex
            // can have multiple texture coordinates so we always take the first
            // set (0).
            vec.x = ai_mesh->mTextureCoords[0][i].x;
            vec.y = ai_mesh->mTextureCoords[0][i].y;
            vertex.texcoords = vec;
            // // tangent
            vector.x = ai_mesh->mTangents[i].x;
            vector.y = ai_mesh->mTangents[i].y;
            vector.z = ai_mesh->mTangents[i].z;
            vertex.tangent = vector;
            // bitangent
            vector.x = ai_mesh->mBitangents[i].x;
            vector.y = ai_mesh->mBitangents[i].y;
            vector.z = ai_mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        } else {
            vertex.texcoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle)
    // and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
        aiFace face = ai_mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    // process materials
    aiMaterial *material = scene->mMaterials[ai_mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse
    // texture should be named as 'texture_diffuseN' where N is a sequential
    // number ranging from 1 to MAX_SAMPLER_NUMBER. Same applies to other
    // texture as the following list summarizes: diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    model_load_material_textures(model, &textures, material,
                                 aiTextureType_DIFFUSE, "texture_diffuse");
    // // 2. specular maps
    model_load_material_textures(model, &textures, material,
                                 aiTextureType_SPECULAR, "texture_specular");
    // 3. normal maps
    model_load_material_textures(model, &textures, material,
                                 aiTextureType_HEIGHT, "texture_normal");
    // 4. height maps
    model_load_material_textures(model, &textures, material,
                                 aiTextureType_AMBIENT, "texture_height");


    // return a mesh object created from the extracted mesh data
    mesh_create(mesh, vertices, indices, textures);
}

void model_load_material_textures(model_t *model,
                                  std::vector<texture_t> *textures,
                                  aiMaterial *mat, aiTextureType type,
                                  std::string type_name) {
    std::vector<texture_t> tex_map;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next
        // iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < model->textures_loaded.size(); j++) {
            if (std::strcmp(model->textures_loaded[j].path.data(),
                            str.C_Str()) == 0) {
                textures->push_back(model->textures_loaded[j]);
                // a texture with the same filepath has already been loaded,
                // continue to next one. (optimization)
                skip = true;
                break;
            }
        }
        if (!skip) {
            texture_t texture;
            texture.path = str.C_Str();
            texture.type = type_name;

            std::string filename = std::string(str.C_Str());
            filename = model->directory + '/' + filename;

            texture_create(&texture, filename.c_str(), type_name,
                           GL_UNSIGNED_BYTE);
            tex_map.push_back(texture);
            model->textures_loaded.push_back(texture);
        }
    }

    textures->insert(textures->end(), tex_map.begin(), tex_map.end());
}
