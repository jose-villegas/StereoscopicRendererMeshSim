#include "SceneObject.h"
using namespace Scene;

SceneObject::SceneObject(unsigned int index)
{
    this->ID = index;
    this->base = new Bases::BaseObject();
}

Scene::SceneObject::SceneObject(unsigned int index, const std::string &sObjectName)
{
    this->ID = index;
    this->base = new Bases::BaseObject(sObjectName);
}

void Scene::SceneObject::addComponent(Bases::BaseComponent *component)
{
    components.push_back(component);
}

void Scene::SceneObject::removeComponent(const unsigned int &componentIndex)
{
    if (!components[componentIndex]) { return; }

    components.erase(components.begin() + componentIndex);
}
