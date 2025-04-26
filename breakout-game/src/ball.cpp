#include "ball.h"
#include "game_object.h"

void ball_create(ball_t *ball, glm::vec2 pos, float radius, texture_t *sprite,
                 glm::vec3 color, glm::vec2 velocity) {
    game_object_t game_object;
    game_object_create(&game_object, pos, glm::vec2(radius * 2.0f), sprite,
                       color, velocity);
    ball->game_object = game_object;
    ball->radius = BALL_RADIUS;
    ball->stuck = true;
    ball->sticky = false;
    ball->pass_through = false;
}

void ball_update(ball_t *ball, float dt, unsigned int window_width) {
    // if not stuck to player board
    if (!ball->stuck) {
        // move the ball
        ball->game_object.position += ball->game_object.velocity * dt;
        // then check if outside window bounds and if so, reverse velocity and
        // restore at correct position
        if (ball->game_object.position.x <= 0.0f) {
            ball->game_object.velocity.x = -ball->game_object.velocity.x;
            ball->game_object.position.x = 0.0f;
        } else if (ball->game_object.position.x + ball->game_object.size.x >=
                   window_width) {
            ball->game_object.velocity.x = -ball->game_object.velocity.x;
            ball->game_object.position.x =
                window_width - ball->game_object.size.x;
        }
        if (ball->game_object.position.y <= 0.0f) {
            ball->game_object.velocity.y = -ball->game_object.velocity.y;
            ball->game_object.position.y = 0.0f;
        }
    }
}

// resets the ball to initial Stuck Position (if ball is outside window bounds)
void ball_reset(ball_t *ball, glm::vec2 pos, glm::vec2 velocity) {
    ball->game_object.position = pos;
    ball->game_object.velocity = velocity;
    ball->game_object.color = glm::vec3(1.0f);
    ball->stuck = true;
    ball->sticky = false;
    ball->pass_through = false;
}
