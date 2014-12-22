#include "SceneObject.h"
using namespace scene;

SceneObject::SceneObject()
{
}

void scene::SceneObject::addComponent(bases::BaseComponent *component)
{
    components.push_back(component);
}

void scene::SceneObject::removeComponent(const unsigned int &componentIndex)
{
    if (!components[componentIndex]) { return; }

    components.erase(components.begin() + componentIndex);
}

void scene::SceneObject::setBaseObject(bases::BaseObject *base)
{
    this->base = base;
}
