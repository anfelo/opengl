#ifndef CAMERA_H_
#define CAMERA_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

typedef struct camera_t {
    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    // euler Angles
    float yaw;
    float pitch;
    // camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;
} camera_t;

void camera_create(camera_t *camera, glm::vec3 position, glm::vec3 up,
                   float yaw, float pitch);
glm::mat4 camera_get_view_matrix(camera_t *camera);
// processes input received from any keyboard-like input system. Accepts input
// parameter in the form of camera defined ENUM (to abstract it from windowing
// systems)
void camera_process_keyboard(camera_t *camera, CameraMovement direction,
                             float deltaTime);
// processes input received from a mouse input system. Expects the offset
// value in both the x and y direction.
void camera_process_mouse_movement(camera_t *camera, float xoffset,
                                   float yoffset, GLboolean constrainPitch);
void camera_process_mouse_scroll(camera_t *camera, float yoffset);
#endif
