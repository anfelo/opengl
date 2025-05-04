#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game.h"
#include "GLFW/glfw3.h"
#include "ball.h"
#include "game_level.h"
#include "game_object.h"
#include "particle_generator.h"
#include "post_processor.h"
#include "powerup.h"
#include "resource_manager.h"
#include "shader.h"
#include "sprite_renderer.h"

float shake_time = 0.0f;

// game constructor
void game_create(game_t *game, unsigned int width, unsigned int height) {
    game->width = width;
    game->height = height;
    game->state = GAME_ACTIVE;
    game->lives = GAME_LIVES;
}

// initialize game state (load all shaders/textures/levels)
void game_init(game_t *game) {
    // load shaders
    rm_load_shader(game->resources, "./resources/shaders/sprite.vert",
                   "./resources/shaders/sprite.frag", nullptr, "sprite");
    rm_load_shader(game->resources, "./resources/shaders/particle.vert",
                   "./resources/shaders/particle.frag", nullptr, "particle");
    rm_load_shader(game->resources, "./resources/shaders/post_processing.vert",
                   "./resources/shaders/post_processing.frag", nullptr,
                   "post_processing");
    rm_load_shader(game->resources, "./resources/shaders/text_2d.vert",
                   "./resources/shaders/text_2d.frag", nullptr, "text");

    shader_program_t *sprite_shader = rm_get_shader(game->resources, "sprite");
    shader_program_t *particle_shader =
        rm_get_shader(game->resources, "particle");
    shader_program_t *text_shader = rm_get_shader(game->resources, "text");

    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(game->width),
                   static_cast<float>(game->height), 0.0f, -1.0f, 1.0f);
    glm::mat4 text_projection =
        glm::ortho(0.0f, static_cast<float>(game->width),
                   static_cast<float>(game->height), 0.0f);

    shader_program_activate(sprite_shader);
    shader_set_int(sprite_shader, "u_image", 0);
    shader_set_mat4(sprite_shader, "u_projection", projection);

    shader_program_activate(particle_shader);
    shader_set_int(particle_shader, "u_sprite", 0);
    shader_set_mat4(particle_shader, "u_projection", projection);

    shader_program_activate(text_shader);
    shader_set_int(particle_shader, "u_text", 0);
    shader_set_mat4(particle_shader, "u_projection", text_projection);

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
    rm_load_texture(game->resources, "./resources/textures/particle.png",
                    GL_RGBA, "particle");
    rm_load_texture(game->resources, "./resources/textures/powerup_speed.png",
                    GL_RGBA, "powerup_speed");
    rm_load_texture(game->resources, "./resources/textures/powerup_sticky.png",
                    GL_RGBA, "powerup_sticky");
    rm_load_texture(game->resources,
                    "./resources/textures/powerup_increase.png", GL_RGBA,
                    "powerup_increase");
    rm_load_texture(game->resources, "./resources/textures/powerup_confuse.png",
                    GL_RGBA, "powerup_confuse");
    rm_load_texture(game->resources, "./resources/textures/powerup_chaos.png",
                    GL_RGBA, "powerup_chaos");
    rm_load_texture(game->resources,
                    "./resources/textures/powerup_passthrough.png", GL_RGBA,
                    "powerup_passthrough");

    // set render-specific controls
    sr_create(game->renderer, sprite_shader);
    particle_generator_create(game->particle_generator, particle_shader,
                              rm_get_texture(game->resources, "particle"), 500);
    post_processor_create(game->effects,
                          rm_get_shader(game->resources, "post_processing"),
                          game->width, game->height);

    // text rendering loading
    text_renderer_create(game->text_renderer, text_shader, game->width,
                         game->height);
    text_renderer_load(game->text_renderer, "./resources/fonts/ocraext.TTF",
                       24);

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
        if (game->keys[GLFW_KEY_A] || game->keys[GLFW_KEY_LEFT]) {
            if (game->player.position.x >= 0.0f) {
                game->player.position.x -= velocity;
                if (game->ball.stuck) {
                    game->ball.game_object.position.x -= velocity;
                }
            }
        }
        if (game->keys[GLFW_KEY_D] || game->keys[GLFW_KEY_RIGHT]) {
            if (game->player.position.x <= game->width - game->player.size.x) {
                game->player.position.x += velocity;
                if (game->ball.stuck) {
                    game->ball.game_object.position.x += velocity;
                }
            }
        }
        if (game->keys[GLFW_KEY_SPACE]) {
            game->ball.stuck = false;
        }
    }
    if (game->state == GAME_MENU) {
        if (game->keys[GLFW_KEY_ENTER] &&
            !game->keys_processed[GLFW_KEY_ENTER]) {
            game->state = GAME_ACTIVE;
            game->keys_processed[GLFW_KEY_ENTER] = true;
        }
        if (game->keys[GLFW_KEY_W] && !game->keys_processed[GLFW_KEY_W]) {
            game->level = (game->level + 1) % 4;
            game->keys_processed[GLFW_KEY_W] = true;
        }
        if (game->keys[GLFW_KEY_S] && !game->keys_processed[GLFW_KEY_S]) {
            if (game->level > 0) {
                game->level -= 1;
            } else {
                game->level = 3;
            }
            game->keys_processed[GLFW_KEY_S] = true;
        }
    }
    if (game->state == GAME_WIN) {
        if (game->keys[GLFW_KEY_ENTER]) {
            game->keys_processed[GLFW_KEY_ENTER] = true;
            game->effects->chaos = false;
            game->state = GAME_MENU;
        }
    }
}

void game_update(game_t *game, float dt) {
    ball_update(&game->ball, dt, game->width);

    game_do_collisions(game);

    // update particles
    particle_generator_update(game->particle_generator, dt,
                              &game->ball.game_object, 2,
                              glm::vec2(game->ball.radius / 2.0f));

    game_update_powerups(game, dt);

    // reduce shake time
    if (shake_time > 0.0f) {
        shake_time -= dt;
        if (shake_time <= 0.0f) {
            game->effects->shake = false;
        }
    }

    // check win condition
    if (game->state == GAME_ACTIVE &&
        game_level_is_complete(&game->levels[game->level])) {
        game_reset_level(game);
        game_reset_player(game);
        game->effects->chaos = true;
        game->state = GAME_WIN;
    }

    // check loss condition
    if (game->ball.game_object.position.y >= game->height) {
        game->lives -= 1;

        if (game->lives == 0) {
            game_reset_level(game);
            game->state = GAME_MENU;
        }

        game_reset_player(game);
    }
}

void game_render(game_t *game) {
    if (game->state == GAME_ACTIVE || game->state == GAME_MENU ||
        game->state == GAME_WIN) {
        // begin rendering to postprocessing framebuffer
        post_processor_begin_render(game->effects);

        // draw background
        texture_t *bg_texture = rm_get_texture(game->resources, "background");
        sr_draw_sprite(game->renderer, bg_texture, glm::vec2(0.0f, 0.0f),
                       glm::vec2(game->width, game->height), 0.0f);

        // draw level
        game_level_draw(game->renderer, &game->levels[game->level]);

        // player
        game_object_draw(game->renderer, &game->player);

        // draw powerups
        for (powerup_t &powerup : game->powerups) {
            if (!powerup.game_object.destroyed) {
                game_object_draw(game->renderer, &powerup.game_object);
            }
        }

        // particles (particles are on top of all the other objects but
        // below the ball)
        particle_generator_draw(game->particle_generator);

        // ball
        game_object_draw(game->renderer, &game->ball.game_object);

        std::stringstream ss;
        ss << game->lives;
        render_text(game->text_renderer, "Lives:" + ss.str(), 5.0f, 5.0f, 1.0f,
                    glm::vec3(1.0f));

        // end rendering to postprocessing framebuffer
        post_processor_end_render(game->effects);
        // render postprocessing quad
        post_processor_render(game->effects, glfwGetTime());
    }
    if (game->state == GAME_MENU) {
        render_text(game->text_renderer, "Press ENTER to start", 250.0f,
                    game->height / 2.0f, 1.0f, glm::vec3(1.0f));
        render_text(game->text_renderer, "Press W or S to select level", 245.0f,
                    game->height / 2.0f + 20.0f, 0.75f, glm::vec3(1.0f));
    }
    if (game->state == GAME_WIN) {
        render_text(game->text_renderer, "You WON!!", 320.0f,
                    game->height / 2.0f - 20.0f, 1.0f,
                    glm::vec3(0.0f, 1.0f, 0.0f));
        render_text(game->text_renderer, "Press ENTER to retry or ESC to quit",
                    130.0f, game->height / 2.0f, 1.0f,
                    glm::vec3(1.0f, 1.0f, 0.0f));
    }
}

void game_reset_level(game_t *game) {
    if (game->level == 0) {
        game_level_load(game->resources, &game->levels[0],
                        "./resources/levels/one.lvl", game->width,
                        game->height / 2);
    } else if (game->level == 1) {
        game_level_load(game->resources, &game->levels[1],
                        "./resources/levels/two.lvl", game->width,
                        game->height / 2);
    } else if (game->level == 2) {
        game_level_load(game->resources, &game->levels[2],
                        "./resources/levels/three.lvl", game->width,
                        game->height / 2);
    } else if (game->level == 3) {
        game_level_load(game->resources, &game->levels[3],
                        "./resources/levels/four.lvl", game->width,
                        game->height / 2);
    }

    // Reset the game lives
    game->lives = GAME_LIVES;
}

void game_reset_player(game_t *game) {
    // reset player/ball stats
    game->player.size = glm::vec2(100.0f, 20.0f);
    game->player.position =
        glm::vec2(game->width / 2.0f - game->player.size.x / 2.0f,
                  game->height - game->player.size.y);
    game->player.color = glm::vec3(1.0f);

    glm::vec2 ball_pos = game->player.position +
                         glm::vec2(game->player.size.x / 2.0f - BALL_RADIUS,
                                   -(BALL_RADIUS * 2.0f));
    ball_reset(&game->ball, ball_pos, INITIAL_BALL_VELOCITY);
}

// collision detection
bool check_collision_AABB_AABB(game_object_t one, game_object_t two);
collision_t check_collision_AABB_circle(ball_t one, game_object_t two);
direction_t vector_direction(glm::vec2 closest);

void game_do_collisions(game_t *game) {
    for (game_object_t &box : game->levels[game->level].bricks) {
        if (!box.destroyed) {
            collision_t collision =
                check_collision_AABB_circle(game->ball, box);
            if (collision.collided) {
                // destroy block if not solid
                if (!box.is_solid) {
                    box.destroyed = true;
                    game_spawn_powerups(game, &box);
                } else {
                    shake_time = 0.05f;
                    game->effects->shake = true;
                }
                // collision resolution
                direction_t dir = collision.direction;
                glm::vec2 diff_vector = collision.diff_vec;
                // don't do collision resolution on non-solid bricks if
                // pass-through is activated
                if (!(game->ball.pass_through && !box.is_solid)) {
                    // horizontal collision
                    if (dir == LEFT || dir == RIGHT) {
                        // reverse horizontal velocity
                        game->ball.game_object.velocity.x =
                            -game->ball.game_object.velocity.x;
                        // relocate
                        float penetration =
                            game->ball.radius - std::abs(diff_vector.x);
                        if (dir == LEFT) {
                            // move ball to right
                            game->ball.game_object.position.x += penetration;
                        } else {
                            // move ball to left;
                            game->ball.game_object.position.x -= penetration;
                        }
                    } else {
                        // vertical collision
                        // reverse vertical velocity
                        game->ball.game_object.velocity.y =
                            -game->ball.game_object.velocity.y;
                        // relocate
                        float penetration =
                            game->ball.radius - std::abs(diff_vector.y);
                        if (dir == UP) {
                            // move ball back up
                            game->ball.game_object.position.y -= penetration;
                        } else {
                            // move ball back down
                            game->ball.game_object.position.y += penetration;
                        }
                    }
                }
            }
        }
    }

    for (powerup_t &powerup : game->powerups) {
        if (!powerup.game_object.destroyed) {
            if (powerup.game_object.position.y >= game->height) {
                powerup.game_object.destroyed = true;
            }

            // collided with player, now activate powerup
            if (check_collision_AABB_AABB(game->player, powerup.game_object)) {
                game_activate_powerup(game, &powerup);
                powerup.game_object.destroyed = true;
                powerup.activated = true;
            }
        }
    }

    // check collisions for player pad (unless stuck)
    collision_t result = check_collision_AABB_circle(game->ball, game->player);
    if (!game->ball.stuck && result.collided) {
        // check where it hit the board, and change velocity based on where it
        // hit the board
        float centerBoard =
            game->player.position.x + game->player.size.x / 2.0f;
        float distance =
            (game->ball.game_object.position.x + game->ball.radius) -
            centerBoard;
        float percentage = distance / (game->player.size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 old_velocity = game->ball.game_object.velocity;
        game->ball.game_object.velocity.x =
            INITIAL_BALL_VELOCITY.x * percentage * strength;
        // ball->velocity.y = -ball->velocity.y;
        game->ball.game_object.velocity =
            // keep speed consistent over both axes
            // (multiply by length of old velocity, so
            // total strength is not changed)
            glm::normalize(game->ball.game_object.velocity) *
            glm::length(old_velocity);
        // fix sticky paddle
        game->ball.game_object.velocity.y =
            -1.0f * abs(game->ball.game_object.velocity.y);

        // if Sticky powerup is activated, also stick ball to paddle once new
        // velocity vectors were calculated
        game->ball.stuck = game->ball.sticky;
    }
}

// AABB - AABB collision
bool check_collision_AABB_AABB(game_object_t one, game_object_t two) {
    // collision x-axis?
    bool collision_x = one.position.x + one.size.x >= two.position.x &&
                       two.position.x + two.size.x >= one.position.x;
    // collision y-axis?
    bool collision_y = one.position.y + one.size.y >= two.position.y &&
                       two.position.y + two.size.y >= one.position.y;
    // collision only if on both axes
    return collision_x && collision_y;
}

// AABB - Circle collision
collision_t check_collision_AABB_circle(ball_t one, game_object_t two) {
    // get center point circle first
    glm::vec2 center(one.game_object.position + one.radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabb_center(two.position.x + aabb_half_extents.x,
                          two.position.y + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped =
        glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to
    // circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if
    // length <= radius
    difference = closest - center;

    // not <= since in that case a collision also occurs when
    // object one exactly touches object two, which they are at
    // the end of each collision resolution stage.
    if (glm::length(difference) < one.radius) {
        return (collision_t){
            true,
            vector_direction(difference),
            difference,
        };
    } else {
        return (collision_t){
            false,
            UP,
            glm::vec2(0.0f),
        };
    }
}

// calculates which direction a vector is facing (N,E,S or W)
direction_t vector_direction(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // up
        glm::vec2(1.0f, 0.0f),  // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f, 0.0f)  // left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++) {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max) {
            max = dot_product;
            best_match = i;
        }
    }
    return (direction_t)best_match;
}

// Powerups
static inline bool is_other_powerup_active(std::vector<powerup_t> &powerups,
                                           std::string type) {
    // Check if another PowerUp of the same type is still active
    // in which case we don't disable its effect (yet)
    for (const powerup_t &powerup : powerups) {
        if (powerup.activated) {
            if (powerup.type == type) {
                return true;
            }
        }
    }
    return false;
}

static inline bool should_spawn(unsigned int chance) {
    unsigned int random = rand() % chance;
    return random == 0;
}

void game_spawn_powerups(game_t *game, game_object_t *block) {
    // 1 in 75 chance
    if (should_spawn(75)) {
        powerup_t powerup;
        powerup_create(&powerup, "speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f,
                       block->position,
                       rm_get_texture(game->resources, "powerup_speed"));
        game->powerups.push_back(powerup);
    }
    if (should_spawn(75)) {
        powerup_t powerup;
        powerup_create(&powerup, "sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f,
                       block->position,
                       rm_get_texture(game->resources, "powerup_sticky"));
        game->powerups.push_back(powerup);
    }
    if (should_spawn(75)) {
        powerup_t powerup;
        powerup_create(&powerup, "pass-through", glm::vec3(0.5f, 1.0f, 0.5f),
                       10.0f, block->position,
                       rm_get_texture(game->resources, "powerup_passthrough"));
        game->powerups.push_back(powerup);
    }
    if (should_spawn(75)) {
        powerup_t powerup;
        powerup_create(&powerup, "pad-size-increase",
                       glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, block->position,
                       rm_get_texture(game->resources, "powerup_increase"));
        game->powerups.push_back(powerup);
    }
    // negative powerups should spawn more often
    if (should_spawn(15)) {
        powerup_t powerup;
        powerup_create(&powerup, "confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f,
                       block->position,
                       rm_get_texture(game->resources, "powerup_confuse"));
        game->powerups.push_back(powerup);
    }
    if (should_spawn(15)) {
        powerup_t powerup;
        powerup_create(&powerup, "chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f,
                       block->position,
                       rm_get_texture(game->resources, "powerup_chaos"));
        game->powerups.push_back(powerup);
    }
}

void game_update_powerups(game_t *game, float dt) {
    for (powerup_t &powerup : game->powerups) {
        powerup.game_object.position += powerup.game_object.velocity * dt;
        if (powerup.activated) {
            powerup.duration -= dt;

            if (powerup.duration <= 0.0f) {
                // remove powerup from list (will later be removed)
                powerup.activated = false;
                // deactivate effects
                if (powerup.type == "sticky") {
                    // only reset if no other powerup of
                    // type sticky is active
                    if (!is_other_powerup_active(game->powerups, "sticky")) {
                        game->ball.sticky = false;
                        game->player.color = glm::vec3(1.0f);
                    }
                } else if (powerup.type == "pass-through") {
                    // only reset if no other powerup
                    // of type pass-through is active
                    if (!is_other_powerup_active(game->powerups,
                                                 "pass-through")) {
                        game->ball.pass_through = false;
                        game->ball.game_object.color = glm::vec3(1.0f);
                    }
                } else if (powerup.type == "confuse") {
                    // only reset if no other powerup of
                    // type confuse is active
                    if (!is_other_powerup_active(game->powerups, "confuse")) {
                        game->effects->confuse = false;
                    }
                } else if (powerup.type == "chaos") {
                    // only reset if no other powerup of
                    // type chaos is active
                    if (!is_other_powerup_active(game->powerups, "chaos")) {
                        game->effects->chaos = false;
                    }
                }
            }
        }
    }
    // remove all powerups from vector that are destroyed and !activated (thus
    // either off the map or finished) note we use a lambda expression to remove
    // each powerup which is destroyed and not activated
    game->powerups.erase(
        std::remove_if(game->powerups.begin(), game->powerups.end(),
                       [](const powerup_t &powerup) {
                           return powerup.game_object.destroyed &&
                                  !powerup.activated;
                       }),
        game->powerups.end());
}

void game_activate_powerup(game_t *game, powerup_t *powerup) {
    if (powerup->type == "speed") {
        game->ball.game_object.velocity *= 1.2;
    } else if (powerup->type == "sticky") {
        game->ball.sticky = true;
        game->player.color = glm::vec3(1.0f, 0.5f, 1.0f);
    } else if (powerup->type == "pass-through") {
        game->ball.pass_through = true;
        game->ball.game_object.color = glm::vec3(1.0f, 0.5f, 0.5f);
    } else if (powerup->type == "pad-size-increase") {
        game->player.size.x += 50;
    } else if (powerup->type == "confuse") {
        if (!game->effects->chaos) {
            // only activate if chaos wasn't already active
            game->effects->confuse = true;
        }
    } else if (powerup->type == "chaos") {
        if (!game->effects->confuse) {
            game->effects->chaos = true;
        }
    }
}
