#include "Transform.h"
using namespace types;

Transform::Transform(void)
{
}

glm::mat4 types::Transform::getModelMatrix()
{
    return glm::mat4(glm::translate(this->position) * glm::mat4_cast(this->rotation) * glm::scale(this->scale));
}