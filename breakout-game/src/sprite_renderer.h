#ifndef SPRITE_RENDERER_H_
#define SPRITE_RENDERER_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VAO.h"
#include "shader.h"
#include "texture.h"

typedef struct sprite_renderer_t {
    shader_program_t *shader;
    VAO_t quad_VAO;
} sprite_renderer_t;

void sr_create(sprite_renderer_t *renderer, shader_program_t *shader);
void sr_destroy(sprite_renderer_t *renderer);
void sr_draw_sprite(sprite_renderer_t *renderer, texture_t *texture,
                    glm::vec2 position,
                    glm::vec2 size = glm::vec2(10.0f, 10.0f),
                    float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

#endif
