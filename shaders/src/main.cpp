#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

GLfloat vertices[] = {
    // Position          // Color
    0.5f,  0.5f,  0.0f,  1.0f, 0.0f, 0.0f, // top right
    0.5f,  -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,// bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,// bottom left
    -0.5f, 0.5f,  0.0f,  0.0f, 1.0f, 1.0f// top left
};
GLuint indices[] = {
    // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    shader_program_t shader_program;
    shader_program_create(&shader_program, "./resources/shaders/default.vert", "resources/shaders/default.frag");

    VAO_t VAO1;
    VAO_create(&VAO1);
    VAO_bind(&VAO1);

    VBO_t VBO1;
    VBO_create(&VBO1, vertices, sizeof(vertices));
    EBO_t EBO1;
    EBO_create(&EBO1, indices, sizeof(indices));

    VAO_link_attrib(&VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO_link_attrib(&VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    VAO_unbind();
    VBO_unbind();
    EBO_unbind();

    // Locates the uniform u_scale in the program
    GLuint uniform_id = glGetUniformLocation(shader_program.ID, "u_scale");

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program_activate(&shader_program);

        // Sets the uniform value
        glUniform1f(uniform_id, 0.5f);

        VAO_bind(&VAO1);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO_delete(&VAO1);
    VBO_delete(&VBO1);
    EBO_delete(&EBO1);
    shader_program_delete(&shader_program);

    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
