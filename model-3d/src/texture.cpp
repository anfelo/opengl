#include "texture.h"

void texture_create(texture_t *tex, const char *texture_file,
                    std::string tex_type, GLenum pixel_type) {
    glGenTextures(1, &tex->ID);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(texture_file, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = 0;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, tex->ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     pixel_type, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
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
