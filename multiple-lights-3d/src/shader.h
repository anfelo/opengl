#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char *filename);

typedef struct shader_program_t {
    GLuint ID;
} shader_program_t;

void shader_program_create(shader_program_t *shader, const char *vertex_file,
                           const char *fragment_file);
void shader_program_activate(shader_program_t *shader);
void shader_program_delete(shader_program_t *shader);

void shader_set_float(shader_program_t *shader, const char *name, float value);
void shader_set_bool(shader_program_t *shader, const char *name, bool value);
void shader_set_int(shader_program_t *shader, const char *name, int Value);

void shader_set_vec3(shader_program_t *shader, const char *name, float *value);
void shader_set_mat4(shader_program_t *shader, const char *name, float *value);

#endif
