#include "Vertex.h"
using namespace types;

Vertex::Vertex(const glm::vec3 &pos, const glm::vec2 &tex, const glm::vec3 &normal, const glm::vec3 &tangent)
{
    this->position = pos;
    this->texCoords = tex;
    this->normal = normal;
    this->tangent = tangent;
}
