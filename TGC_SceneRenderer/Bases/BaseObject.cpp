#include "BaseObject.h"
using namespace Bases;

BaseObject::BaseObject(void)
{
    objectName = "SceneObject";
}

Bases::BaseObject::BaseObject(const std::string &sObjectName)
{
    this->objectName = objectName;
}


