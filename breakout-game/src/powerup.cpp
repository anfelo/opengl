#include "powerup.h"

void powerup_create(powerup_t *powerup, std::string type, glm::vec3 color,
                    float duration, glm::vec2 position, texture_t *texture) {
    game_object_t game_object;
    game_object_create(&game_object, position, POWERUP_SIZE, texture, color,
                       POWERUP_VELOCITY);
    powerup->game_object = game_object;
    powerup->type = type;
    powerup->duration = duration;
    powerup->activated = false;
}
