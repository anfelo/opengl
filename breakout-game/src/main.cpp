#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

game_t game;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window =
        glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float delta_time = 0.0f;
    float last_frame = 0.0f;

    game_create(&game, SCREEN_WIDTH, SCREEN_HEIGHT);

    resource_manager_t resources;
    game.resources = &resources;
    sprite_renderer_t renderer;
    game.renderer = &renderer;

    game_init(&game);

    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        glfwPollEvents();

        // manage user input
        // -----------------
        game_process_input(&game, delta_time);

        // update game state
        // -----------------
        game_update(&game, delta_time);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        game_render(&game);

        glfwSwapBuffers(window);
    }

    // deletes all resources as loaded using the resource manager
    // ----------------------------------------------------------
    rm_clear_resources(game.resources);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            game.keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            game.keys[key] = false;
        }
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
