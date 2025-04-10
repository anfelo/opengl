#include "texture.h"

void texture_create(texture_t *tex, const char *texture_file, GLenum tex_type,
                    GLenum slot, GLenum format, GLenum pixel_type) {
    tex->type = tex_type;

    glGenTextures(1, &tex->ID);
    glActiveTexture(slot);
    glBindTexture(tex->type, tex->ID);

    // set the texture wrapping/filtering options (on the currently bound
    // texture object)
    glTexParameteri(tex->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(tex->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(tex->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(tex->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(texture_file, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(tex->type, 0, format, width, height, 0, format, pixel_type,
                     data);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Unbinds the OpenGL Texture object so that it can't accidentally be
    // modified
    glBindTexture(tex->type, 0);
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
