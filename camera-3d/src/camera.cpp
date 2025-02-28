#include "camera.h"

static void update_camera_vectors(camera_t *camera);

void camera_create(camera_t *camera, glm::vec3 position, glm::vec3 up,
                   float yaw, float pitch) {
    camera->front = glm::vec3(0.0f, 0.0f, -1.0f);
    camera->movement_speed = SPEED;
    camera->mouse_sensitivity = SENSITIVITY;
    camera->zoom = ZOOM;
    camera->position = position;
    camera->world_up = up;
    camera->yaw = yaw;
    camera->pitch = pitch;

    update_camera_vectors(camera);
}

glm::mat4 camera_get_view_matrix(camera_t *camera) {
    return glm::lookAt(camera->position, camera->position + camera->front,
                       camera->up);
}

void camera_process_keyboard(camera_t *camera, CameraMovement direction,
                             float deltaTime) {
float velocity = camera->movement_speed * deltaTime;

    if (direction == FORWARD) {
        camera->position += camera->front * velocity;
    }
    if (direction == BACKWARD) {
        camera->position -= camera->front * velocity;
    }
    if (direction == LEFT) {
        camera->position -= camera->right * velocity;
    }
    if (direction == RIGHT) {
        camera->position += camera->right * velocity;
    }
}

void camera_process_mouse_movement(camera_t *camera, float xoffset,
                                   float yoffset, GLboolean constrainPitch) {
    xoffset *= camera->mouse_sensitivity;
    yoffset *= camera->mouse_sensitivity;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (camera->pitch > 89.0f) {
            camera->pitch = 89.0f;
        }
        if (camera->pitch < -89.0f) {
            camera->pitch = -89.0f;
        }
    }

    update_camera_vectors(camera);
}

void camera_process_mouse_scroll(camera_t *camera, float yoffset) {
    camera->zoom -= (float)yoffset;
    if (camera->zoom < 1.0f) {
        camera->zoom = 1.0f;
    }
    if (camera->zoom > 45.0f) {
        camera->zoom = 45.0f;
    }
}

static void update_camera_vectors(camera_t *camera) {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    front.y = sin(glm::radians(camera->pitch));
    front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    // normalize the vectors, because their length gets
    // closer to 0 the more you look up or down which
    // results in slower movement.
    camera->right = glm::normalize(glm::cross(camera->front, camera->world_up));
    camera->up = glm::normalize(glm::cross(camera->right, camera->front));
}
