#include "Transform.h"
using namespace Types;

Transform::Transform(void)
{
}

glm::mat4 Types::Transform::getModelMatrix()
{
    return glm::mat4(glm::translate(this->position) * glm::mat4_cast(this->rotation) * glm::scale(this->scale));
}