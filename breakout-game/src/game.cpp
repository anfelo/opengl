#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game.h"
#include "GLFW/glfw3.h"
#include "ball.h"
#include "game_object.h"
#include "resource_manager.h"
#include "shader.h"
#include "sprite_renderer.h"

// game constructor
void game_create(game_t *game, unsigned int width, unsigned int height) {
    game->width = width;
    game->height = height;
    game->state = GAME_ACTIVE;
}

// initialize game state (load all shaders/textures/levels)
void game_init(game_t *game) {
    // load shaders
    rm_load_shader(game->resources, "./resources/shaders/sprite.vert",
                   "./resources/shaders/sprite.frag", nullptr, "sprite");

    shader_program_t *shader = rm_get_shader(game->resources, "sprite");

    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(game->width),
                   static_cast<float>(game->height), 0.0f, -1.0f, 1.0f);

    shader_program_activate(shader);
    shader_set_int(shader, "u_image", 0);
    shader_set_mat4(shader, "u_projection", projection);

    // set render-specific controls
    sr_create(game->renderer, shader);

    // load textures
    rm_load_texture(game->resources, "./resources/textures/background.jpg",
                    GL_RGB, "background");
    rm_load_texture(game->resources, "./resources/textures/awesomeface.png",
                    GL_RGBA, "face");
    rm_load_texture(game->resources, "./resources/textures/block.png", GL_RGB,
                    "block");
    rm_load_texture(game->resources, "./resources/textures/block_solid.png",
                    GL_RGB, "block_solid");
    rm_load_texture(game->resources, "./resources/textures/paddle.png", GL_RGBA,
                    "paddle");

    // load levels
    game_level_t one;
    game_level_load(game->resources, &one, "./resources/levels/one.lvl",
                    game->width, game->height / 2);
    game_level_t two;
    game_level_load(game->resources, &two, "./resources/levels/two.lvl",
                    game->width, game->height / 2);
    game_level_t three;
    game_level_load(game->resources, &three, "./resources/levels/three.lvl",
                    game->width, game->height / 2);
    game_level_t four;
    game_level_load(game->resources, &four, "./resources/levels/four.lvl",
                    game->width, game->height / 2);

    game->levels.push_back(one);
    game->levels.push_back(two);
    game->levels.push_back(three);
    game->levels.push_back(four);
    game->level = 0;

    // Init player paddle
    game_object_t player;
    glm::vec2 player_size = glm::vec2(100.0f, 20.0f);
    float player_velocity = 500.0f;
    glm::vec2 player_pos = glm::vec2(game->width / 2.0f - player_size.x / 2.0f,
                                     game->height - player_size.y);
    game_object_create(&player, player_pos, player_size,
                       rm_get_texture(game->resources, "paddle"),
                       glm::vec3(1.0f), glm::vec2(player_velocity, 0.0f));
    game->player = player;

    // Init ball object
    ball_t ball;
    glm::vec2 ball_pos =
        player_pos +
        glm::vec2(player.size.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    glm::vec2 ball_velocity = INITIAL_BALL_VELOCITY;
    ball_create(&ball, ball_pos, BALL_RADIUS,
                rm_get_texture(game->resources, "face"), glm::vec3(1.0f),
                ball_velocity);
    game->ball = ball;
}

// game loop
void game_process_input(game_t *game, float dt) {
    if (game->state == GAME_ACTIVE) {
        float velocity = game->player.velocity.x * dt;
        // move playerboard
        if (game->keys[GLFW_KEY_A]) {
            if (game->player.position.x >= 0.0f) {
                game->player.position.x -= velocity;
            }
        }
        if (game->keys[GLFW_KEY_D]) {
            if (game->player.position.x <= game->width - game->player.size.x) {
                game->player.position.x += velocity;
            }
        }
        if (game->keys[GLFW_KEY_SPACE]) {
            game->ball.stuck = false;
        }
    }
}

void game_update(game_t *game, float dt) {
    ball_update(&game->ball, dt, game->width);
}

void game_render(game_t *game) {
    if (game->state == GAME_ACTIVE) {
        // draw background
        texture_t *bg_texture = rm_get_texture(game->resources, "background");
        sr_draw_sprite(game->renderer, bg_texture, glm::vec2(0.0f, 0.0f),
                       glm::vec2(game->width, game->height), 0.0f);

        // draw level
        game_level_draw(game->renderer, &game->levels[game->level]);

        // player
        game_object_draw(game->renderer, &game->player);

        // ball
        game_object_draw(game->renderer, &game->ball.game_object);
    }
}
