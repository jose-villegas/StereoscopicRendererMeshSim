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

scene::SceneObject::~SceneObject()
{
    for (unsigned int i = 0; i < this->components.size(); i++) {
        bases::BaseComponent *ptr = this->components[i];

        if (dynamic_cast<scene::Mesh *>(ptr) != 0) {
            collections::MeshesCollection::Instance()->removeMesh((scene::Mesh *)ptr);
        } else if (dynamic_cast<scene::Light *>(ptr) != 0) {
            collections::LightsCollection::Instance()->removeLight((scene::Light *)ptr);
        } else if (dynamic_cast<scene::Camera *>(ptr) != 0) {
            collections::CamerasCollection::Instance()->removeCamera((scene::Camera *)ptr);
        }
    }

    delete base;
    this->components.clear();
}
