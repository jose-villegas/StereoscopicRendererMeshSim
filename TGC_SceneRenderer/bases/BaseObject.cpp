#include "BaseObject.h"
using namespace bases;

BaseObject::BaseObject(void)
{
    objectName = "SceneObject";
}

bases::BaseObject::BaseObject(const std::string &sObjectName)
{
    this->objectName = objectName;
}


