#ifndef BALL_H_
#define BALL_H_

#include "game_object.h"

#define BALL_RADIUS 12.5f

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

typedef struct ball_t {
    game_object_t game_object;
    float radius;
    bool stuck;
    bool sticky, pass_through;
} ball_t;

void ball_create(ball_t *ball, glm::vec2 pos, float radius, texture_t *sprite,
                 glm::vec3 color, glm::vec2 velocity);
void ball_update(ball_t *ball, float dt, unsigned int window_width);
void ball_reset(ball_t *ball, glm::vec2 pos, glm::vec2 velocity);

#endif
