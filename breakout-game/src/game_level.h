#ifndef GAME_LEVEL_H_
#define GAME_LEVEL_H_

#include <vector>
#include "game_object.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

typedef struct game_level_t {
    std::vector<game_object_t> bricks;
} game_level_t;

void game_level_load(resource_manager_t *resources, game_level_t *level,
                     const char *file, unsigned int level_width,
                     unsigned int level_height);
void game_level_draw(sprite_renderer_t *renderer, game_level_t *level);
bool game_level_is_complete(game_level_t *level);
void game_level_init(resource_manager_t *resources, game_level_t *level,
                     std::vector<std::vector<unsigned int>> tile_data,
                     unsigned int level_width, unsigned int level_height);

#endif
