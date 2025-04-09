#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game.h"
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
    rm_load_texture(game->resources, "./resources/textures/awesomeface.png",
                    GL_RGBA, "face");
}

// game loop
void game_process_input(game_t *game, float dt) {}
void game_update(game_t *game, float dt) {}

void game_render(game_t *game) {
    texture_t *texture = rm_get_texture(game->resources, "face");
    sr_draw_sprite(game->renderer, texture, glm::vec2(200.0f, 200.0f),
                   glm::vec2(300.0f, 400.0f), 45.0f,
                   glm::vec3(0.0f, 1.0f, 0.0f));
}
