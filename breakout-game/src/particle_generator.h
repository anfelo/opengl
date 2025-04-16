#ifndef PARTICLE_GENERATOR_H_
#define PARTICLE_GENERATOR_H_

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"

typedef struct particle_t {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;
    float life;
} particle_t;

void particle_create(particle_t *particle);

typedef struct particle_generator_t {
    // state
    std::vector<particle_t> particles;
    unsigned int amount;
    unsigned int last_used_particle;

    // render state
    shader_program_t *shader;
    texture_t *texture;
    VAO_t VAO;

} particle_generator_t;

void particle_generator_create(particle_generator_t *pg,
                               shader_program_t *shader, texture_t *texture,
                               unsigned int amount);
void particle_generator_update(particle_generator_t *pg, float dt,
                               game_object_t *object,
                               unsigned int new_particles, glm::vec2 offset);
void particle_generator_draw(particle_generator_t *pg);

#endif
