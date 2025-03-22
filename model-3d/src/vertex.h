#ifndef VERTEX_H_
#define VERTEX_H_

#include <glm/glm.hpp>

#define MAX_BONE_INFLUENCE 4

typedef struct vertex_t {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    // bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
} vertex_t;

#endif
