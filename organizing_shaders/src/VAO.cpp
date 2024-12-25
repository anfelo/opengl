#include "VAO.h"

void VAO_create(VAO_t *VAO) {
    glGenVertexArrays(1, &VAO->ID);
}

void VAO_link_VBO(VBO_t *VBO, GLuint layout) {
    VBO_bind(VBO);
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(layout);
    VBO_unbind();
}

void VAO_bind(VAO_t *VAO) {
    glBindVertexArray(VAO->ID);
}

void VAO_unbind() {
    glBindVertexArray(0);
}

void VAO_delete(VAO_t *VAO) {
    glDeleteVertexArrays(1, &VAO->ID);
}
