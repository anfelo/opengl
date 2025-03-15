#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture.h"

#include <iostream>

#include "VAO.h"
#include "VBO.h"

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
    -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

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

// lighting
float light_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float point0_light_color[4] = {0.05f, 0.05f, 0.05f, 1.0f};
float point1_light_color[4] = {0.05f, 0.05f, 0.05f, 1.0f};
float point2_light_color[4] = {0.05f, 0.05f, 0.05f, 1.0f};
float point3_light_color[4] = {0.05f, 0.05f, 0.05f, 1.0f};

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

    // Setup Dear ImGui context
    // -----------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    shader_program_t lighting_shader_program;
    shader_program_create(&lighting_shader_program,
                          "./resources/shaders/default.vert",
                          "resources/shaders/default.frag");
    shader_program_t light_cube_shader_program;
    shader_program_create(&light_cube_shader_program,
                          "./resources/shaders/light_cube.vert",
                          "resources/shaders/light_cube.frag");

    VAO_t cube_VAO;
    VAO_create(&cube_VAO);
    VAO_bind(&cube_VAO);

    VBO_t VBO;
    VBO_create(&VBO, vertices, sizeof(vertices));
    VAO_link_attrib(&VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
    VAO_link_attrib(&VBO, 1, 3, GL_FLOAT, 8 * sizeof(float),
                    (void *)(3 * sizeof(float)));
    VAO_link_attrib(&VBO, 2, 2, GL_FLOAT, 8 * sizeof(float),
                    (void *)(6 * sizeof(float)));

    // second, configure the light's VAO (VBO stays the same; the vertices are
    // the same for the light object which is also a 3D cube)
    VAO_t light_cube_VAO;
    VAO_create(&light_cube_VAO);
    VAO_bind(&light_cube_VAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer),
    // no need to fill it; the VBO's data already contains all we need (it's
    // already bound, but we do it again for educational purposes)
    VAO_link_attrib(&VBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);

    camera_create(&camera, glm::vec3(0.0f, 0.0f, 3.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH);

    texture_t diffuse_map;
    texture_create(&diffuse_map, "./resources/textures/container.png",
                   GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture_uniform(&lighting_shader_program, "u_material.diffuse", 0);

    texture_t specular_map;
    texture_create(&specular_map, "./resources/textures/container_specular.png",
                   GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);
    texture_uniform(&lighting_shader_program, "u_material.specular", 1);

    glm::vec3 cube_positions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glm::vec3 point_light_positions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f),
    };

    shader_program_activate(&lighting_shader_program);
    shader_set_int(&lighting_shader_program, "u_material.diffuse", 0);
    shader_set_int(&lighting_shader_program, "u_material.specular", 1);

    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // input
        // --------------------
        if (!io.WantCaptureMouse) {
            process_input(window);
        }

        // render
        // --------------------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader_program_activate(&lighting_shader_program);
        shader_set_vec3(&lighting_shader_program, "u_view_pos",
                        glm::value_ptr(camera.position));
        shader_set_float(&lighting_shader_program, "u_material.shininess",
                         32.0f);

        glm::vec3 light_color_vec =
            glm::vec3(light_color[0], light_color[1], light_color[2]);
        glm::vec3 diffuse_color_vec = light_color_vec * glm::vec3(0.5f);
        glm::vec3 ambient_color_vec = diffuse_color_vec * glm::vec3(0.2f);
        glm::vec3 specular_color_vec = glm::vec3(1.0f, 1.0f, 1.0f);

        shader_set_vec3(&lighting_shader_program, "u_light.ambient",
                        glm::value_ptr(ambient_color_vec));
        shader_set_vec3(&lighting_shader_program, "u_light.diffuse",
                        glm::value_ptr(diffuse_color_vec));
        shader_set_vec3(&lighting_shader_program, "u_light.specular",
                        glm::value_ptr(specular_color_vec));
        shader_set_vec3(&lighting_shader_program, "u_light.position",
                        glm::value_ptr(camera.position));
        shader_set_vec3(&lighting_shader_program, "u_light.direction",
                        glm::value_ptr(camera.front));
        shader_set_float(&lighting_shader_program, "u_light.cut_off",
                         glm::cos(glm::radians(12.5f)));
        shader_set_float(&lighting_shader_program, "u_light.outer_cut_off",
                         glm::cos(glm::radians(17.5f)));

        // Light attenuation constants
        shader_set_float(&lighting_shader_program, "u_light.constant", 1.0f);
        shader_set_float(&lighting_shader_program, "u_light.linear", 0.09f);
        shader_set_float(&lighting_shader_program, "u_light.quadratic", 0.032f);
        // directional light
        glm::vec3 directional_light_dir =
            glm::vec3(light_color[0], light_color[1], light_color[2]);
        glm::vec3 directional_light_ambient = glm::vec3(0.05f, 0.05f, 0.05f);
        glm::vec3 directional_light_diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
        glm::vec3 directional_light_specular = glm::vec3(0.5f, 0.5f, 0.5f);
        shader_set_vec3(&lighting_shader_program, "u_dir_light.direction",
                        glm::value_ptr(directional_light_dir));
        shader_set_vec3(&lighting_shader_program, "u_dir_light.ambient",
                        glm::value_ptr(directional_light_ambient));
        shader_set_vec3(&lighting_shader_program, "u_dir_light.diffuse",
                        glm::value_ptr(directional_light_diffuse));
        shader_set_vec3(&lighting_shader_program, "u_dir_light.specular",
                        glm::value_ptr(directional_light_specular));

        // point light 1
        glm::vec3 point0_light_ambient =
            glm::vec3(point0_light_color[0], point0_light_color[1],
                      point0_light_color[2]);
        glm::vec3 point0_light_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 point0_light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
        shader_set_vec3(&lighting_shader_program, "u_point_lights[0].position",
                        glm::value_ptr(point_light_positions[0]));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[0].ambient",
                        glm::value_ptr(point0_light_ambient));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[0].diffuse",
                        glm::value_ptr(point0_light_diffuse));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[0].specular",
                        glm::value_ptr(point0_light_specular));
        shader_set_float(&lighting_shader_program, "u_point_lights[0].constant",
                         1.0f);
        shader_set_float(&lighting_shader_program, "u_point_lights[0].linear",
                         0.09f);
        shader_set_float(&lighting_shader_program,
                         "u_point_lights[0].quadratic", 0.032f);

        // point light 2
        glm::vec3 point1_light_ambient =
            glm::vec3(point1_light_color[0], point1_light_color[1],
                      point1_light_color[2]);
        glm::vec3 point1_light_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 point1_light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
        shader_set_vec3(&lighting_shader_program, "u_point_lights[1].position",
                        glm::value_ptr(point_light_positions[1]));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[1].ambient",
                        glm::value_ptr(point1_light_ambient));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[1].diffuse",
                        glm::value_ptr(point1_light_diffuse));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[1].specular",
                        glm::value_ptr(point1_light_specular));
        shader_set_float(&lighting_shader_program, "u_point_lights[1].constant",
                         1.0f);
        shader_set_float(&lighting_shader_program, "u_point_lights[1].linear",
                         0.09f);
        shader_set_float(&lighting_shader_program,
                         "u_point_lights[1].quadratic", 0.032f);

        // point light 3
        glm::vec3 point2_light_ambient =
            glm::vec3(point2_light_color[0], point2_light_color[1],
                      point2_light_color[2]);
        glm::vec3 point2_light_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 point2_light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
        shader_set_vec3(&lighting_shader_program, "u_point_lights[2].position",
                        glm::value_ptr(point_light_positions[2]));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[2].ambient",
                        glm::value_ptr(point2_light_ambient));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[2].diffuse",
                        glm::value_ptr(point2_light_diffuse));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[2].specular",
                        glm::value_ptr(point2_light_specular));
        shader_set_float(&lighting_shader_program, "u_point_lights[2].constant",
                         1.0f);
        shader_set_float(&lighting_shader_program, "u_point_lights[2].linear",
                         0.09f);
        shader_set_float(&lighting_shader_program,
                         "u_point_lights[2].quadratic", 0.032f);

        // point light 4
        glm::vec3 point3_light_ambient =
            glm::vec3(point3_light_color[0], point3_light_color[1],
                      point3_light_color[2]);
        glm::vec3 point3_light_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 point3_light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
        shader_set_vec3(&lighting_shader_program, "u_point_lights[3].position",
                        glm::value_ptr(point_light_positions[3]));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[3].ambient",
                        glm::value_ptr(point3_light_ambient));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[3].diffuse",
                        glm::value_ptr(point3_light_diffuse));
        shader_set_vec3(&lighting_shader_program, "u_point_lights[3].specular",
                        glm::value_ptr(point3_light_specular));
        shader_set_float(&lighting_shader_program, "u_point_lights[3].constant",
                         1.0f);
        shader_set_float(&lighting_shader_program, "u_point_lights[3].linear",
                         0.09f);
        shader_set_float(&lighting_shader_program,
                         "u_point_lights[3].quadratic", 0.032f);

        // spotLight
        glm::vec3 spot_light_ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 spot_light_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 spot_light_specular = glm::vec3(1.0f, 1.0f, 1.0f);
        shader_set_vec3(&lighting_shader_program, "u_spot_light.position",
                        glm::value_ptr(camera.position));
        shader_set_vec3(&lighting_shader_program, "u_spot_light.direction",
                        glm::value_ptr(camera.front));
        shader_set_vec3(&lighting_shader_program, "u_spot_light.ambient",
                        glm::value_ptr(spot_light_ambient));
        shader_set_vec3(&lighting_shader_program, "u_spot_light.diffuse",
                        glm::value_ptr(spot_light_diffuse));
        shader_set_vec3(&lighting_shader_program, "u_spot_light.specular",
                        glm::value_ptr(spot_light_specular));
        shader_set_float(&lighting_shader_program, "u_spot_light.constant",
                         1.0f);
        shader_set_float(&lighting_shader_program, "u_spot_light.linear",
                         0.09f);
        shader_set_float(&lighting_shader_program, "u_spot_light.quadratic",
                         0.032f);
        shader_set_float(&lighting_shader_program, "u_spot_light.cut_off",
                         glm::cos(glm::radians(12.5f)));
        shader_set_float(&lighting_shader_program, "u_spot_light.outer_cut_off",
                         glm::cos(glm::radians(15.0f)));

        // view/projection transformations
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.zoom),
            (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera_get_view_matrix(&camera);

        shader_set_mat4(&lighting_shader_program, "u_projection",
                        glm::value_ptr(projection));
        shader_set_mat4(&lighting_shader_program, "u_view",
                        glm::value_ptr(view));

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        shader_set_mat4(&lighting_shader_program, "u_model",
                        glm::value_ptr(model));

        texture_bind(&diffuse_map, GL_TEXTURE0);
        texture_bind(&specular_map, GL_TEXTURE1);

        VAO_bind(&cube_VAO);
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));
            shader_set_mat4(&lighting_shader_program, "u_model",
                            glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp object(s)
        shader_program_activate(&light_cube_shader_program);
        shader_set_mat4(&light_cube_shader_program, "u_projection",
                        glm::value_ptr(projection));
        shader_set_mat4(&light_cube_shader_program, "u_view",
                        glm::value_ptr(view));

        // we now draw as many light bulbs as we have point lights.
        VAO_bind(&light_cube_VAO);
        for (unsigned int i = 0; i < 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, point_light_positions[i]);
            model =
                glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            shader_set_mat4(&light_cube_shader_program, "u_model",
                            glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        ImGui::Begin("Properties");
        ImGui::Text("Light");
        ImGui::ColorEdit4("Light Color", light_color);
        ImGui::Text("Point Light 1");
        ImGui::ColorEdit4("Light Color 1", point0_light_color);
        ImGui::Text("Point Light 2");
        ImGui::ColorEdit4("Light Color 2", point1_light_color);
        ImGui::Text("Point Light 3");
        ImGui::ColorEdit4("Light Color 3", point2_light_color);
        ImGui::Text("Point Light 4");
        ImGui::ColorEdit4("Light Color 4", point3_light_color);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    VAO_delete(&light_cube_VAO);
    VAO_delete(&cube_VAO);
    VBO_delete(&VBO);
    shader_program_delete(&lighting_shader_program);
    shader_program_delete(&light_cube_shader_program);

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

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_process_keyboard(&camera, RIGHT, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera_process_keyboard(&camera, UP, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera_process_keyboard(&camera, DOWN, delta_time);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // INFO: Hide the mouse cursor on left mouse button click
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (first_click) {
            // Center the cursor and reset the last mouse position
            glfwSetCursorPos(window, (SCREEN_WIDTH / 2.0f),
                             (SCREEN_HEIGHT / 2.0f));
            last_x = SCREEN_WIDTH / 2.0f;
            last_y = SCREEN_HEIGHT / 2.0f;
            first_click = false;
        }

        double mouse_x;
        double mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        float xoffset = (float)(mouse_x)-last_x;
        float yoffset = last_y - (float)(mouse_y);

        last_x = mouse_x;
        last_y = mouse_y;

        camera_process_mouse_movement(&camera, xoffset, yoffset, true);

        // Reset the mouse position and the last mouse position
        // to calculate the next frame's delta
        glfwSetCursorPos(window, (SCREEN_WIDTH / 2.0f), (SCREEN_HEIGHT / 2.0f));
        last_x = SCREEN_WIDTH / 2.0f;
        last_y = SCREEN_HEIGHT / 2.0f;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
               GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        first_click = true;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// INFO: This callback is not used at the moment as we are doing the the
// mouse movement while processing the inputs
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (first_click) {
        last_x = xpos;
        last_y = ypos;
        first_click = false;
    }

    float xoffset = xpos - last_x;
    float yoffset =
        last_y - ypos; // reversed since y-coordinates go from bottom to top

    last_x = xpos;
    last_y = ypos;

    camera_process_mouse_movement(&camera, xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera_process_mouse_scroll(&camera, static_cast<float>(yoffset));
}
