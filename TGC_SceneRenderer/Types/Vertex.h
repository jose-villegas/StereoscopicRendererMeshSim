#pragma once
#include "GLM\glm.hpp"
namespace Types {
    class Vertex {
        public:
            glm::vec3 position;
            glm::vec2 texCoords;
            glm::vec3 normal;
            Vertex(void);
            Vertex(const glm::vec3 &pos, const glm::vec2 &tex, const glm::vec3 &normal);
    };
}
