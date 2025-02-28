#ifndef EBO_H_
#define EBO_H_

#include <glad/glad.h>

typedef struct EBO_t {
    GLuint ID;
} EBO_t;

void EBO_create(EBO_t *EBO, GLuint *indices, GLsizeiptr size);
void EBO_bind(EBO_t *EBO);
void EBO_unbind();
void EBO_delete(EBO_t *EBO);

#endif
