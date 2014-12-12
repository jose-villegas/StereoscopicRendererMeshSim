#include "Vertex.h"
using namespace Types;

Vertex::Vertex(void)
{
}

Vertex::Vertex(const glm::vec3 &pos, const glm::vec2 &tex, const const glm::vec3 &normal)
{
    m_pos = pos;
    m_tex = tex;
    m_normal = normal;
}
