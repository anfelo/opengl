#ifndef MESH_H_
#define MESH_H_

#include <vector>

#include "shader.h"
#include "texture.h"
#include "vertex.h"

typedef struct mesh_t {
    std::vector<vertex_t> vertices;
    std::vector<GLuint> indices;
    std::vector<texture_t> textures;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
} mesh_t;

void mesh_create(mesh_t *mesh, std::vector<vertex_t> vertices,
                 std::vector<GLuint> indices, std::vector<texture_t> textures);
void mesh_setup(mesh_t *mesh);
void mesh_draw(shader_program_t *shader, mesh_t *mesh);

#endif
