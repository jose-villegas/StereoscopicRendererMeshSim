#include "BaseObject.h"
using namespace Scene;

BaseObject::BaseObject(void)
{
    objectName = "SceneObject";
}

glm::mat4 BaseObject::getModelMatrix()
{
    return glm::mat4(glm::translate(this->position) * glm::mat4_cast(this->rotation) * glm::scale(this->scale));
}
