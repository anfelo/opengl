#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

#include "shader.h"

typedef struct texture_t {
    GLuint ID;
    GLenum type;
    GLenum internal_format;
    GLenum image_format;
    int width;
    int height;
} texture_t;

void texture_create(texture_t *texture);
void texture_load(texture_t *texture, const char *texture_file, bool alpha);
void texture_generate(texture_t *texture, int width, int height,
                      unsigned char *data);
void texture_bind(texture_t *tex, GLenum slot);
void texture_uniform(shader_program_t *shader, const char *uniform,
                     GLuint unit);
void texture_unbind(texture_t *tex);
void texture_delete(texture_t *tex);

#endif
