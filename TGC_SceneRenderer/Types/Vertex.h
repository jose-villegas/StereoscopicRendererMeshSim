#pragma once
#include "GLM\glm.hpp"
namespace Types {
    class Vertex {
        public:
            glm::vec3 m_pos;
            glm::vec2 m_tex;
            glm::vec3 m_normal;
            Vertex(void);
            Vertex(const glm::vec3 &pos, const glm::vec2 &tex, const const glm::vec3 &normal);
    };
}

