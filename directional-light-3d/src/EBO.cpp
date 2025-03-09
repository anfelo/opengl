#include "EBO.h"

void EBO_create(EBO_t *EBO, GLuint* indices, GLsizeiptr size) {
    glGenBuffers(1, &EBO->ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO->ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO_bind(EBO_t *EBO) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO->ID);
}

void EBO_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO_delete(EBO_t *EBO) {
    glDeleteBuffers(1, &EBO->ID);
}
