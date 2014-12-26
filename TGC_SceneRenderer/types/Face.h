#pragma once
#include "Vertex.h"
#include "glm\glm.hpp"
#include <array>

namespace types {
    class Vertex; // forward declaration to avoid circular header dependency
}

namespace types {

    class Face {
        public:
            // engine only works with triangular faces
            std::array <types::Vertex *, 3> vertices;
            std::array <unsigned int, 3> indices;
            glm::vec3 normal;
            void calculateNormal();
            bool hasVertex(types::Vertex *v);
            // construtors
            Face(void);
            ~Face() {};
            Face(
                types::Vertex &v1,
                types::Vertex &v2,
                types::Vertex &v3,
                const unsigned int i1,
                const unsigned int i2,
                const unsigned int i3,
                const bool calcNormal = false
            );
    };
}

