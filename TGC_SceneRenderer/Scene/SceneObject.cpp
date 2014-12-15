#include "SceneObject.h"
using namespace Scene;

SceneObject::SceneObject(void)
{
    this->objectInstance = new Bases::BaseObject();
    this->objectTransform = new Types::Transform();
}