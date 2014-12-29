#include "Transform.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

using namespace types;

Transform::Transform(void)
{
    this->position = glm::vec3(0.0, 0.0, 0.0);
    this->rotation = glm::quat(glm::vec3(0.0, 0.0, 0.0));
    this->scale = glm::vec3(1.0, 1.0, 1.0);
}

glm::mat4 types::Transform::getModelMatrix()
{
    return glm::mat4(glm::translate(this->position) * glm::mat4_cast(this->rotation) * glm::scale(this->scale));
}

void types::Transform::setPosition(const float &value0, const float &value1, const float &value2)
{
    this->position.x = value0;
    this->position.y = value1;
    this->position.z = value2;
}

void types::Transform::setRotation(const float &value0, const float &value1, const float &value2)
{
    this->rotation = glm::quat(glm::vec3(value0, value1, value2));
}

void types::Transform::setScale(const float &value0, const float &value1, const float &value2)
{
    this->scale.x = value0;
    this->scale.y = value1;
    this->scale.z = value2;
}

glm::vec3 types::Transform::eulerAngles()
{
    return glm::eulerAngles(this->rotation);
}
