#include "BaseObject.h"
using namespace bases;

BaseObject::BaseObject(void) : objectName("SceneObject"), active(true), transform(types::Transform())
{
}

bases::BaseObject::BaseObject(const std::string &sObjectName) : objectName(sObjectName), active(true), transform(types::Transform())
{
}
