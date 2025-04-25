#include "post_processor.h"
#include "shader.h"

#include <iostream>

void post_processor_create(post_processor_t *post_processor,
                           shader_program_t *shader, unsigned int width,
                           unsigned int height) {
    texture_t texture;
    texture_create(&texture);
    post_processor->texture = texture;

    post_processor->shader = shader;
    post_processor->width = width;
    post_processor->height = height;

    post_processor->confuse = false;
    post_processor->shake = false;
    post_processor->chaos = false;

    // initialize renderbuffer/framebuffer object
    glGenFramebuffers(1, &post_processor->MSFBO);
    glGenFramebuffers(1, &post_processor->FBO);
    glGenRenderbuffers(1, &post_processor->RBO);
    // initialize renderbuffer storage with a multisampled color buffer (don't
    // need a depth/stencil buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, post_processor->MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, post_processor->RBO);
    glRenderbufferStorageMultisample(
        GL_RENDERBUFFER, 4, GL_RGB, width,
        height); // allocate storage for render buffer object
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
        post_processor->RBO); // attach MS render buffer object to framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO"
                  << std::endl;
    // also initialize the FBO/texture to blit multisampled color-buffer to;
    // used for shader operations (for postprocessing effects)
    glBindFramebuffer(GL_FRAMEBUFFER, post_processor->FBO);
    texture_generate(&post_processor->texture, width, height, NULL);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        post_processor->texture.ID,
        0); // attach texture to framebuffer as its color attachment
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO"
                  << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // initialize render data and uniforms
    post_processor_init_render_data(post_processor);
    shader_program_activate(post_processor->shader);
    shader_set_int(post_processor->shader, "scene", 0);
    float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        {-offset, offset},  // top-left
        {0.0f, offset},     // top-center
        {offset, offset},   // top-right
        {-offset, 0.0f},    // center-left
        {0.0f, 0.0f},       // center-center
        {offset, 0.0f},     // center - right
        {-offset, -offset}, // bottom-left
        {0.0f, -offset},    // bottom-center
        {offset, -offset}   // bottom-right
    };
    glUniform2fv(glGetUniformLocation(post_processor->shader->ID, "offsets"), 9,
                 (float *)offsets);
    int edge_kernel[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
    glUniform1iv(
        glGetUniformLocation(post_processor->shader->ID, "edge_kernel"), 9,
        edge_kernel);
    float blur_kernel[9] = {1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
                            2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
                            1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f};
    glUniform1fv(
        glGetUniformLocation(post_processor->shader->ID, "blur_kernel"), 9,
        blur_kernel);
}

void post_processor_begin_render(post_processor_t *post_processor) {
    glBindFramebuffer(GL_FRAMEBUFFER, post_processor->MSFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void post_processor_end_render(post_processor_t *post_processor) {
    // now resolve multisampled color-buffer into intermediate FBO to store to
    // texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, post_processor->MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, post_processor->FBO);
    glBlitFramebuffer(0, 0, post_processor->width, post_processor->height, 0, 0,
                      post_processor->width, post_processor->height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    // binds both READ and WRITE framebuffer to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void post_processor_render(post_processor_t *post_processor, float time) {
    // set uniforms/options
    shader_program_activate(post_processor->shader);
    shader_set_float(post_processor->shader, "time", time);
    shader_set_int(post_processor->shader, "confuse", post_processor->confuse);
    shader_set_int(post_processor->shader, "chaos", post_processor->chaos);
    shader_set_int(post_processor->shader, "shake", post_processor->shake);

    // render textured quad
    glActiveTexture(GL_TEXTURE0);
    texture_bind(&post_processor->texture, GL_TEXTURE0);
    glBindVertexArray(post_processor->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void post_processor_init_render_data(post_processor_t *post_processor) {
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {// pos        // tex
                        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 1.0f,
                        1.0f,  1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,

                        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, -1.0f,
                        1.0f,  0.0f,  1.0f,  1.0f, 1.0f, 1.0f};
    glGenVertexArrays(1, &post_processor->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(post_processor->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
