#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"

GLfloat vertices[] = {
    // positions          // texcoords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// stores how much we're seeing of either texture
float mix_tex_value = 0.2f;

// timing
float delta_time = 0.0f; // time between current frame and last frame
float last_frame = 0.0f;

static camera_t camera;
float last_x = SCREEN_WIDTH / 2.0f;
float last_y = SCREEN_HEIGHT / 2.0f;
bool first_click = true;
bool camera_rotation_enabled = false;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
                                          "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    shader_program_t shader_program;
    shader_program_create(&shader_program, "./resources/shaders/default.vert", "resources/shaders/default.frag");

    VAO_t VAO1;
    VAO_create(&VAO1);
    VAO_bind(&VAO1);

    VBO_t VBO1;
    VBO_create(&VBO1, vertices, sizeof(vertices));

    texture_t texture1;
    texture_create(&texture1, "./resources/textures/wooden_shelf.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    texture_t texture2;
    texture_create(&texture2, "./resources/textures/awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);

    VAO_link_attrib(&VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    VAO_link_attrib(&VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    VAO_unbind();
    VBO_unbind();
    EBO_unbind();

    // Locates the uniform u_tex in the program
    GLuint tex_uniform_id = glGetUniformLocation(shader_program.ID, "u_tex");
    // Locates the uniform u_
    GLuint model_uniform_loc = glGetUniformLocation(shader_program.ID, "u_model");
    GLuint view_uniform_loc = glGetUniformLocation(shader_program.ID, "u_view");
    GLuint projection_uniform_loc = glGetUniformLocation(shader_program.ID, "u_projection");

    shader_program_activate(&shader_program);

    texture_uniform(&shader_program, "texture1", 0);
    texture_uniform(&shader_program, "texture2", 1);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f),
    };

    // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    // -----------------------------------------------------------------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(projection_uniform_loc, 1, GL_FALSE, glm::value_ptr(projection));

    camera_create(&camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH);

    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        glUniform1f(tex_uniform_id, mix_tex_value);

        texture_bind(&texture1, GL_TEXTURE0);
        texture_bind(&texture2, GL_TEXTURE1);

        shader_program_activate(&shader_program);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(projection_uniform_loc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
        glUniformMatrix4fv(view_uniform_loc, 1, GL_FALSE, glm::value_ptr(view));

        VAO_bind(&VAO1);

        // Draw 10 cubes with different positions and rotations
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(model_uniform_loc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO_delete(&VAO1);
    VBO_delete(&VBO1);
    texture_delete(&texture1);
    texture_delete(&texture2);
    shader_program_delete(&shader_program);

    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_process_keyboard(&camera, FORWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_process_keyboard(&camera, BACKWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_process_keyboard(&camera, LEFT, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_process_keyboard(&camera, RIGHT, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera_process_keyboard(&camera, UP, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        camera_process_keyboard(&camera, DOWN, delta_time);
    }


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // INFO: Hide the mouse cursor on left mouse button click
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (first_click) {
            // Center the cursor and reset the last mouse position
            glfwSetCursorPos(window, (SCREEN_WIDTH / 2.0f), (SCREEN_HEIGHT / 2.0f));
            last_x = SCREEN_WIDTH / 2.0f;
            last_y = SCREEN_HEIGHT / 2.0f;
            first_click = false;
        }

        double mouse_x;
        double mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        float xoffset = (float)(mouse_x) - last_x;
        float yoffset = last_y - (float)(mouse_y);

        last_x = mouse_x;
        last_y = mouse_y;

        camera_process_mouse_movement(&camera, xoffset, yoffset, true);

        // Reset the mouse position and the last mouse position
        // to calculate the next frame's delta
        glfwSetCursorPos(window, (SCREEN_WIDTH / 2.0f), (SCREEN_HEIGHT / 2.0f));
        last_x = SCREEN_WIDTH / 2.0f;
        last_y = SCREEN_HEIGHT / 2.0f;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        first_click = true;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// INFO: This callback is not used at the moment as we are doing the the
// mouse movement while processing the inputs
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (first_click)
    {
        last_x = xpos;
        last_y = ypos;
        first_click = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; // reversed since y-coordinates go from bottom to top

    last_x = xpos;
    last_y = ypos;

    camera_process_mouse_movement(&camera, xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera_process_mouse_scroll(&camera, static_cast<float>(yoffset));
}
