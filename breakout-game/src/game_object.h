#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"

typedef struct game_object_t {
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 velocity;
    glm::vec3 color;
    float rotation;
    bool is_solid;
    bool destroyed;
    texture_t *sprite;
} game_object_t;

void game_object_create(game_object_t *game_object, glm::vec2 pos,
                        glm::vec2 size, texture_t *sprite,
                        glm::vec3 color = glm::vec3(1.0f),
                        glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
void game_object_draw(sprite_renderer_t *renderer, game_object_t *game_object);

#endif
