#include "VAO.h"

void VAO_create(VAO_t *VAO) { glGenVertexArrays(1, &VAO->ID); }

void VAO_link_attrib(VBO_t *VBO, GLuint layout, GLuint num_components,
                     GLenum type, GLsizeiptr stride, void *offset) {
  VBO_bind(VBO);
  glVertexAttribPointer(layout, num_components, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  VBO_unbind();
}

void VAO_bind(VAO_t *VAO) { glBindVertexArray(VAO->ID); }

void VAO_unbind() { glBindVertexArray(0); }

void VAO_delete(VAO_t *VAO) { glDeleteVertexArrays(1, &VAO->ID); }
