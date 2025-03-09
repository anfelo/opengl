#include "VBO.h"

void VBO_create(VBO_t *VBO, GLfloat *vertices, GLsizeiptr size) {
    glGenBuffers(1, &VBO->ID);
    glBindBuffer(GL_ARRAY_BUFFER, VBO->ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO_bind(VBO_t *VBO) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO->ID);
}

void VBO_unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO_delete(VBO_t *VBO) {
    glDeleteBuffers(1, &VBO->ID);
}
