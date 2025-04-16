#include "particle_generator.h"
#include "VAO.h"

void particle_create(particle_t *particle) {
    particle->position = glm::vec2(0.0f);
    particle->velocity = glm::vec2(0.0f);
    particle->color = glm::vec4(1.0f);
    particle->life = 0.0f;
}

static void particle_generator_init(particle_generator_t *pg);
static unsigned int first_unused_particle(particle_generator_t *pg);
static void respawn_particle(particle_t *particle, game_object_t *object,
                             glm::vec2 offset);

void particle_generator_create(particle_generator_t *pg,
                               shader_program_t *shader, texture_t *texture,
                               unsigned int amount) {
    pg->shader = shader;
    pg->texture = texture;
    pg->amount = amount;
    particle_generator_init(pg);
}

void particle_generator_update(particle_generator_t *pg, float dt,
                               game_object_t *object,
                               unsigned int new_particles, glm::vec2 offset) {
    for (unsigned int i = 0; i < new_particles; ++i) {
        int unused_particle = first_unused_particle(pg);
        respawn_particle(&pg->particles[unused_particle], object, offset);
    }

    // update all particles
    for (unsigned int i = 0; i < pg->amount; ++i) {
        particle_t *p = &pg->particles[i];
        p->life -= dt;
        if (p->life > 0.0f) {
            p->position -= p->velocity * dt;
            p->color.a -= dt * 2.5f;
        }
    }
}
void particle_generator_draw(particle_generator_t *pg) {
    // use additive blending to give it a glow effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    shader_program_activate(pg->shader);
    for (particle_t particle : pg->particles) {
        if (particle.life > 0.0f) {
            shader_set_vec2(pg->shader, "u_offset", particle.position);
            shader_set_vec4(pg->shader, "u_color", particle.color);

            texture_bind(pg->texture, GL_TEXTURE0);

            VAO_bind(&pg->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            VAO_unbind();
        }
    }
    // don't forget to rest to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static void particle_generator_init(particle_generator_t *pg) {
    // configure VAO/VBO
    VBO_t VBO;
    VAO_t VAO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    pg->VAO = VAO;
    VAO_create(&pg->VAO);
    VBO_create(&VBO, particle_quad, sizeof(particle_quad));

    VAO_bind(&pg->VAO);
    VAO_link_attrib(&VBO, 0, 4, GL_FLOAT, 4 * sizeof(float), (void *)0);

    VBO_unbind();
    VAO_unbind();

    // create this->amount default particle instances
    pg->last_used_particle = 0;
    for (unsigned int i = 0; i < pg->amount; ++i) {
        particle_t particle;
        particle_create(&particle);
        pg->particles.push_back(particle);
    }
}

static unsigned int first_unused_particle(particle_generator_t *pg) {
    // first search from last used particle, this will usually return almost
    // instantly
    for (unsigned int i = pg->last_used_particle; i < pg->amount; ++i) {
        if (pg->particles[i].life <= 0.0f) {
            pg->last_used_particle = i;
            return i;
        }
    }

    // otherwise, do a linear search
    for (unsigned int i = 0; i < pg->last_used_particle; ++i) {
        if (pg->particles[i].life <= 0.0f) {
            pg->last_used_particle = i;
            return i;
        }
    }

    // all particles are taken, override the first one (not that if it
    // repeatedly hits this case, more particles should be reserved)
    pg->last_used_particle = 0;
    return 0;
}

static void respawn_particle(particle_t *particle, game_object_t *object,
                             glm::vec2 offset) {
    float random = ((rand() % 100) - 50) / 10.0f;
    float r_color = 0.5f + ((rand() % 100) / 100.0f);
    particle->position = object->position + random + offset;
    particle->color = glm::vec4(r_color, r_color, r_color, 1.0f);
    particle->life = 1.0f;
    particle->velocity = object->velocity * 0.1f;
}
