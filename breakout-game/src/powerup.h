#ifndef POWERUP_H_
#define POWERUP_H_

#include "game_object.h"

const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
const glm::vec2 POWERUP_VELOCITY(0.0f, 150.0f);

typedef struct powerup_t {
    game_object_t game_object;
    // powerup state
    std::string type;
    float duration;
    bool activated;
} powerup_t;

void powerup_create(powerup_t *powerup, std::string type, glm::vec3 color,
                    float duration, glm::vec2 position, texture_t *texture);

#endif
