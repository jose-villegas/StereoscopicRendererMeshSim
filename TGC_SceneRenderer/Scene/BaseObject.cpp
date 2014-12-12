#include "BaseObject.h"


Scene::BaseObject::BaseObject(void)
{
}

glm::mat4 Scene::BaseObject::getModelMatrix()
{
    return glm::mat4(glm::translate(this->position) * glm::mat4_cast(this->rotation) * glm::scale(this->scale));
}
