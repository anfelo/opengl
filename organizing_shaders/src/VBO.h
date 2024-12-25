#ifndef VBO_H_
#define VBO_H_

#include <glad/glad.h>

typedef struct VBO_t {
    GLuint ID;
} VBO_t;

void VBO_create(VBO_t *VBO, GLfloat *vertices, GLsizeiptr size);
void VBO_bind(VBO_t *VBO);
void VBO_unbind();
void VBO_delete(VBO_t *VBO);

#endif
