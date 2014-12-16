#include "SceneObject.h"
using namespace scene;

SceneObject::SceneObject(unsigned int index)
{
    this->ID = index;
    this->base = new bases::BaseObject();
}

scene::SceneObject::SceneObject(unsigned int index, const std::string &sObjectName)
{
    this->ID = index;
    this->base = new bases::BaseObject(sObjectName);
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
