#ifndef VERTEX_H
#define VERTEX_H

#include "../external/glm.h"

struct Vertex2D {
    glm::vec2 position;
    glm::vec2 uv;
    glm::vec4 color;
    Vertex2D() {}
    Vertex2D(const glm::vec2 &position, const glm::vec2 &uv) {
        this->position = position;
        this->uv = uv;
    }
};

#endif