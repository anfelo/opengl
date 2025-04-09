#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

typedef struct resource_manager_t {
    std::map<std::string, texture_t> textures;
    std::map<std::string, shader_program_t> shaders;
} resource_manager_t;

// loads (and generates) a shader program from file loading vertex, fragment
// (and geometry) shader's source code. If gShaderFile is not nullptr, it
// also loads a geometry shader
void rm_load_shader(resource_manager_t *rm, const char *vertex_shader_file,
                    const char *fragment_shader_file,
                    const char *geometry_shader_file, std::string name);

// retrieves a stored sader
shader_program_t *rm_get_shader(resource_manager_t *rm, std::string name);

// loads (and generates) a texture from file
void rm_load_texture(resource_manager_t *rm, const char *file, GLenum format,
                     std::string name);

// retrieves a stored texture
texture_t *rm_get_texture(resource_manager_t *rm, std::string name);

// properly de-allocates all loaded resources
void rm_clear_resources(resource_manager_t *rm);
#endif
