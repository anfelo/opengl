#include "game_object.h"
#include "sprite_renderer.h"

void game_object_create(game_object_t *game_object, glm::vec2 pos,
                        glm::vec2 size, texture_t *sprite, glm::vec3 color,
                        glm::vec2 velocity) {
    game_object->position = pos;
    game_object->size = size;
    game_object->velocity = velocity;
    game_object->color = color;
    game_object->rotation = 0.0f;
    game_object->is_solid = false;
    game_object->destroyed = false;
    game_object->sprite = sprite;
}

void game_object_draw(sprite_renderer_t *renderer, game_object_t *game_object) {
    sr_draw_sprite(renderer, game_object->sprite, game_object->position,
                   game_object->size, game_object->rotation,
                   game_object->color);
}
