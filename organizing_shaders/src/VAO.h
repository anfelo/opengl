#ifndef VAO_H_
#define VAO_H_

#include <glad/glad.h>
#include "VBO.h"

typedef struct VAO_t {
    GLuint ID;
} VAO_t;

void VAO_create(VAO_t *VAO);
void VAO_link_VBO(VBO_t *VBO, GLuint layout);
void VAO_bind(VAO_t *VAO);
void VAO_unbind();
void VAO_delete(VAO_t *VAO);

#endif
