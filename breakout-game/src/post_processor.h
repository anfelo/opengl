#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader.h"

// post_processor_t hosts all PostProcessing effects for the Breakout
// Game. It renders the game on a textured quad after which one can
// enable specific effects by enabling either the confuse, chaos or
// shake boolean.
// It is required to call begin_render() before rendering the game
// and end_render() after rendering the game for the class to work.
typedef struct post_processor_t {
    shader_program_t *shader;
    texture_t texture;
    unsigned int width, height;
    bool confuse, chaos, shake;
    // render state
    unsigned int MSFBO, FBO; // MSFBO = Multisampled FBO. FBO is regular, used
                             // for blitting MS color-buffer to texture
    unsigned int RBO;        // RBO is used for multisampled color buffer
    unsigned int VAO;
} post_processor_t;

void post_processor_create(post_processor_t *post_processor,
                           shader_program_t *shader, unsigned int width,
                           unsigned int height);
void post_processor_begin_render(post_processor_t *post_processor);
void post_processor_end_render(post_processor_t *post_processor);
void post_processor_render(post_processor_t *post_processor, float time);
void post_processor_init_render_data(post_processor_t *post_processor);

#endif
