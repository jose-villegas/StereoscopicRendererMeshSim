#include "Face.h"
using namespace types;

void types::Face::calculateNormal()
{
    glm::vec3 v0 = this->vertices[0]->position;
    glm::vec3 v1 = this->vertices[1]->position;
    glm::vec3 v2 = this->vertices[2]->position;
    normal = (v1 - v0) * (v2 - v1);

    if (glm::length(normal) == 0) { return; }

    normal = glm::normalize(normal);
}

types::Face::Face(void)
{
}

types::Face::Face(types::Vertex &v1,  types::Vertex &v2, types::Vertex &v3, const bool calcNormal /*= false*/)
{
    this->vertices[0] = &v1;
    this->vertices[1] = &v2;
    this->vertices[2] = &v3;

    if (calcNormal) { this->calculateNormal(); }
}

bool types::Face::hasVertex(types::Vertex *v)
{
    return (v == vertices[0] || v == vertices[1] || v == vertices[2]);
}
