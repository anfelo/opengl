#ifndef GAME_H_
#define GAME_H_

#include "VAO.h"
#include "ball.h"
#include "game_level.h"
#include "particle_generator.h"
#include "post_processor.h"
#include "powerup.h"
#include "resource_manager.h"
#include "shader.h"
#include "sprite_renderer.h"
#include "texture.h"

// Represents the four possible (collision) directions
typedef enum direction_t { UP, RIGHT, DOWN, LEFT } direction_t;
// Defines a Collision typedef that represents collision data
typedef struct collision_t {
    bool collided;
    direction_t direction;
    glm::vec2 diff_vec; // difference vector center - closest point
} collision_t;

enum game_state_t { GAME_ACTIVE, GAME_MENU, GAME_WIN };

typedef struct game_t {
    game_state_t state;
    bool keys[1024];
    unsigned int width, height;

    std::vector<game_level_t> levels;
    unsigned int level;

    game_object_t player;
    ball_t ball;

    std::vector<powerup_t> powerups;

    resource_manager_t *resources;
    sprite_renderer_t *renderer;
    particle_generator_t *particle_generator;
    post_processor_t *effects;
} game_t;

// game constructor
void game_create(game_t *game, unsigned int width, unsigned int height);

// initialize game state (load all shaders/textures/levels)
void game_init(game_t *game);

// game loop
void game_process_input(game_t *game, float dt);
void game_update(game_t *game, float dt);
void game_render(game_t *game);
void game_do_collisions(game_t *game);
void game_reset_level(game_t *game);
void game_reset_player(game_t *game);
// powerups management
void game_spawn_powerups(game_t *game, game_object_t *block);
void game_update_powerups(game_t *game, float dt);
void game_activate_powerup(game_t *game, powerup_t *powerup);

#endif
