#pragma once
#include "Face.h"
#include "glm/glm.hpp"
#include <vector>

namespace types {
    class Face; // forward declaration to avoid circular header dependency
}

namespace types {

    class Vertex {
        public:
            // default members
            glm::vec3 position;
            glm::vec2 texCoords;
            glm::vec3 normal;
            glm::vec3 tangent;
            glm::vec3 bitangent;
            // construtors
            Vertex(const glm::vec3 &pos, const glm::vec2 &tex, const glm::vec3 &normal, const glm::vec3 &tangent, glm::vec3 bitangent);
            Vertex();
            ~Vertex() {};
    };
}

