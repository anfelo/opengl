#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

#include "shader.h"

typedef struct texture_t {
    GLuint ID;
    GLenum type;
} texture_t;

void texture_create(texture_t *tex, const char *texture_file, GLenum tex_type,
                    GLenum slot, GLenum format, GLenum pixel_type);
void texture_bind(texture_t *tex, GLenum slot);
void texture_uniform(shader_program_t *shader, const char *uniform,
                     GLuint unit);
void texture_delete(texture_t *tex);

#endif
