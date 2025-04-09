#include "sprite_renderer.h"
#include "VBO.h"
#include "shader.h"
#include "VAO.h"
#include "VBO.h"

void initRenderData(sprite_renderer_t *renderer);

void sr_create(sprite_renderer_t *renderer, shader_program_t *shader) {
    renderer->shader = shader;
    initRenderData(renderer);
}

void sr_destroy(sprite_renderer_t *renderer) {
    VAO_delete(&renderer->quad_VAO);
}

void sr_draw_sprite(sprite_renderer_t *renderer, texture_t *texture,
                    glm::vec2 position, glm::vec2 size, float rotate,
                    glm::vec3 color) {
    // prepare transformations
    shader_program_activate(renderer->shader);

    glm::mat4 model = glm::mat4(1.0f);

    // first translate (transformations are: scale
    // happens first, then rotation, and then final
    // translation happens; reversed order)
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // move origin of rotation to center of quad
    model =
        glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    // then rotate
    model =
        glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    // move origin back
    model =
        glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    // last scale
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader_set_mat4(renderer->shader, "u_model", model);

    // render textured quad
    shader_set_vec3(renderer->shader, "u_sprite_color", color);

    texture_bind(texture, GL_TEXTURE0);

    VAO_bind(&renderer->quad_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    VAO_unbind();
}

void initRenderData(sprite_renderer_t *renderer) {
    // configure VAO/VBO
    VBO_t VBO;
    VAO_t VAO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    renderer->quad_VAO = VAO;
    VAO_create(&renderer->quad_VAO);
    VBO_create(&VBO, vertices, sizeof(vertices));

    VAO_bind(&renderer->quad_VAO);
    VAO_link_attrib(&VBO, 0, 4, GL_FLOAT, 4 * sizeof(float), (void *)0);

    VBO_unbind();
    VAO_unbind();
}
