#include "game_level.h"
#include "resource_manager.h"

void game_level_load(resource_manager_t *resources, game_level_t *level, const char *file,
                     unsigned int level_width, unsigned int level_height) {
    // clear old data
    level->bricks.clear();
    // load from file
    unsigned int tile_code;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tile_data;
    if (fstream) {
        // read each line from level file
        while (std::getline(fstream, line)) {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            // read each word separated by spaces
            while (sstream >> tile_code) {
                row.push_back(tile_code);
            }
            tile_data.push_back(row);
        }
        if (tile_data.size() > 0) {
            game_level_init(resources, level, tile_data, level_width, level_height);
        }
    }
}

void game_level_draw(sprite_renderer_t *renderer, game_level_t *level) {
    for (game_object_t tile : level->bricks) {
        if (!tile.destroyed) {
            game_object_draw(renderer, &tile);
        }
    }
}

bool game_level_is_complete(game_level_t *level) {
    for (game_object_t tile : level->bricks) {
        if (!tile.is_solid && !tile.destroyed) {
            return false;
        }
    }
    return true;
}

void game_level_init(resource_manager_t *resources, game_level_t *level,
                     std::vector<std::vector<unsigned int>> tile_data,
                     unsigned int level_width, unsigned int level_height) {
    // calculate dimensions
    unsigned int height = tile_data.size();
    // note we can index vector at [0] since this
    // function is only called if height > 0
    unsigned int width = tile_data[0].size();

    float unit_width = level_width / static_cast<float>(width),
          unit_height = level_height / static_cast<float>(height);

    // initialize level tiles based on tileData
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            // check block type from level data (2D level array)
            // Solid Bricks
            if (tile_data[y][x] == 1) {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.7f);
                texture_t *sprite = rm_get_texture(resources, "block_solid");
                game_object_t obj;
                game_object_create(&obj, pos, size, sprite, color);
                obj.is_solid = true;
                level->bricks.push_back(obj);
            } else if (tile_data[y][x] > 1) {
                // non-solid; now determine its color
                // based on level data

                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tile_data[y][x] == 2) {
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                } else if (tile_data[y][x] == 3) {

                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                } else if (tile_data[y][x] == 4) {
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                } else if (tile_data[y][x] == 5) {
                    color = glm::vec3(1.0f, 0.5f, 0.0f);
                }

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                texture_t *sprite = rm_get_texture(resources, "block");
                game_object_t obj;
                game_object_create(&obj, pos, size, sprite, color);
                level->bricks.push_back(obj);
            }
        }
    }
}
