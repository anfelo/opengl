#ifndef GAME_H_
#define GAME_H_

#include "VAO.h"
#include "game_level.h"
#include "resource_manager.h"
#include "shader.h"
#include "sprite_renderer.h"
#include "texture.h"

enum game_state_t { GAME_ACTIVE, GAME_MENU, GAME_WIN };

typedef struct game_t {
    game_state_t state;
    bool keys[1024];
    unsigned int width, height;

    std::vector<game_level_t> levels;
    unsigned int level;

    resource_manager_t *resources;
    sprite_renderer_t *renderer;
} game_t;

// game constructor
void game_create(game_t *game, unsigned int width, unsigned int height);

// initialize game state (load all shaders/textures/levels)
void game_init(game_t *game);

// game loop
void game_process_input(game_t *game, float dt);
void game_update(game_t *game, float dt);
void game_render(game_t *game);

#endif
