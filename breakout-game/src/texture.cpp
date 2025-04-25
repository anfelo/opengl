#include "texture.h"

void texture_create(texture_t *texture) {
    texture->width = 0;
    texture->height = 0;
    texture->internal_format = GL_RGB;
    texture->image_format = GL_RGB;

    glGenTextures(1, &texture->ID);
}

void texture_load(texture_t *texture, const char *texture_file, bool alpha) {
    // create texture object
    if (alpha) {
        texture->internal_format = GL_RGBA;
        texture->image_format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(texture_file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture_generate(texture, width, height, data);
    // and finally free image data
    stbi_image_free(data);
}

void texture_generate(texture_t *texture, int width, int height,
                      unsigned char *data) {
    texture->width = width;
    texture->height = height;
    // create Texture
    glBindTexture(GL_TEXTURE_2D, texture->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->internal_format, width, height, 0,
                 texture->image_format, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_uniform(shader_program_t *shader, const char *uniform,
                     GLuint unit) {
    GLuint tex_uni = glGetUniformLocation(shader->ID, uniform);
    shader_program_activate(shader);
    glUniform1i(tex_uni, unit);
}

void texture_bind(texture_t *tex, GLenum slot) {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, tex->ID);
}

void texture_unbind(texture_t *tex) {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_delete(texture_t *tex) {
    glDeleteTextures(1, &tex->ID);
}
