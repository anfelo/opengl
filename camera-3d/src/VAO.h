#ifndef VAO_H_
#define VAO_H_

#include "VBO.h"
#include <glad/glad.h>

typedef struct VAO_t {
  GLuint ID;
} VAO_t;

void VAO_create(VAO_t *VAO);
void VAO_link_attrib(VBO_t *VBO, GLuint layout, GLuint num_components,
                     GLenum type, GLsizeiptr stride, void *offset);
void VAO_bind(VAO_t *VAO);
void VAO_unbind();
void VAO_delete(VAO_t *VAO);

#endif
