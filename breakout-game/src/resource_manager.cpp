#include "resource_manager.h"

#include "stb_image.h"
#include "texture.h"
#include "shader.h"

// loads (and generates) a shader program from file loading vertex, fragment
// (and geometry) shader's source code. If gShaderFile is not nullptr, it
// also loads a geometry shader
void rm_load_shader(resource_manager_t *rm, const char *vertex_shader_file,
                    const char *fragment_shader_file,
                    const char *geometry_shader_file, std::string name) {
    shader_program_t shader;
    shader_program_create(&shader, vertex_shader_file, fragment_shader_file);
    rm->shaders[name] = shader;
}

// retrieves a stored shader
shader_program_t *rm_get_shader(resource_manager_t *rm, std::string name) {
    return &rm->shaders[name];
}

// loads (and generates) a texture from file
void rm_load_texture(resource_manager_t *rm, const char *file, GLenum format,
                     std::string name) {
    texture_t texture;
    texture_create(&texture);
    texture_load(&texture, file, format == GL_RGBA);
    rm->textures[name] = texture;
}

// retrieves a stored texture
texture_t *rm_get_texture(resource_manager_t *rm, std::string name) {
    return &rm->textures[name];
}

// properly de-allocates all loaded resources
void rm_clear_resources(resource_manager_t *rm) {
    // (properly) delete all shaders
    for (auto iter : rm->shaders) {
        glDeleteProgram(iter.second.ID);
    }
    // (properly) delete all textures
    for (auto iter : rm->textures) {
        glDeleteTextures(1, &iter.second.ID);
    }
}
