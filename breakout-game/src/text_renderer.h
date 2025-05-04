#ifndef TEXT_RENDERER_H_
#define TEXT_RENDERER_H_

#include <glm/glm.hpp>
#include <map>
#include "shader.h"

/// Holds all state information relevant to a character as loaded using FreeType
typedef struct character_t {
    unsigned int texture_ID; // ID handle of the glyph texture
    glm::ivec2 size;         // size of glyph
    glm::ivec2 bearing;      // offset from baseline to left/top of glyph
    unsigned int advance;    // horizontal offset to advance to next glyph
} character_t;

typedef struct text_renderer_t {
    std::map<char, character_t> characters;
    shader_program_t *text_shader;
    unsigned int VAO, VBO;
} text_renderer_t;

void text_renderer_create(text_renderer_t *renderer, shader_program_t *shader,
                          unsigned int width, unsigned int height);
void text_renderer_load(text_renderer_t *renderer, std::string font,
                        unsigned int font_size);
void render_text(text_renderer_t *renderer, std::string text, float x, float y,
                 float scale, glm::vec3 color);

#endif
