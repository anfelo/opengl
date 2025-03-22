#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <assimp/scene.h>
#include "mesh.h"
#include "shader.h"

typedef struct model_t {
    std::vector<texture_t> textures_loaded;
    std::vector<mesh_t> meshes;
    std::string directory;
    bool gamma_correction;
} model_t;

void model_create(model_t *model, const char *path, bool gamma_correction);
void model_load(model_t *model, std::string path);
void model_draw(shader_program_t *shader, model_t *model);
void model_process_node(model_t *model, aiNode *node, const aiScene *scene);
void model_process_mesh(model_t *model, mesh_t *mesh, aiMesh *ai_mesh,
                        const aiScene *scene);
void model_load_material_textures(model_t *model,
                                  std::vector<texture_t> *textures,
                                  aiMaterial *mat, aiTextureType type,
                                  std::string type_name);

#endif
