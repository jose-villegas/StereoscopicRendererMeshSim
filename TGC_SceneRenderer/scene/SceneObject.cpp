#include "SceneObject.h"
#include "Camera.h"
#include "..\collections\LightsCollection.h"
#include "..\collections\MeshesCollection.h"
#include "Mesh.h"

using namespace scene;

SceneObject::SceneObject()
{
}

void scene::SceneObject::addComponent(bases::BaseComponent *component)
{
    for (unsigned int i = 0; i < this->components.size(); i++) {
        bases::BaseComponent *ptr = this->components[i];

        if (typeid(*ptr) == typeid(*component)) {
            std::cout << "SceneObject(" << this << ") " << "cannot have the same component twice, operation cancelled" << std::endl;
            return;
        }
    }

    components.push_back(component);
}

void scene::SceneObject::removeComponent(const unsigned int &componentIndex)
{
    if (componentIndex >= components.size() || !components[componentIndex]) { return; }

    components.erase(components.begin() + componentIndex);
}

void scene::SceneObject::setBaseObject(bases::BaseObject *base)
{
    this->base = base;
}

scene::SceneObject::~SceneObject()
{
    for (unsigned int i = 0; i < this->components.size(); i++) {
        delete this->components[i];
    }

    delete base;
    this->components.clear();
}
