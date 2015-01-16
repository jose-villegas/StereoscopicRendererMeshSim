#include "Plane.h"
#include "glm\detail\func_geometric.hpp"
using namespace types;

Plane::Plane(void)
{
}


Plane::~Plane(void)
{
}

void types::Plane::setPlane(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3)
{
    glm::vec3 aux1, aux2;
    aux1 = v1 - v2;
    aux2 = v3 - v2;
    normal = glm::normalize(aux2 * aux1);
    point = v2;
    planeDistance = -glm::dot(normal, point);
}

void types::Plane::setPlane(const glm::vec3 &position, const glm::vec3 &normal)
{
    this->normal = glm::normalize(normal);
    this->point = position;
    planeDistance = -glm::dot(this->normal, this->point);
}

float types::Plane::distance(glm::vec3 position)
{
    return planeDistance + glm::dot(normal, position);
}
