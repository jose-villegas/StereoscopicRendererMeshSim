#include "SceneObjects.h"
using namespace collections;

SceneObjects::SceneObjects(void)
{
}

SceneObjects::~SceneObjects()
{
}

SceneObjects *SceneObjects::Instance()
{
    if (!_eInstance) {
        _eInstance = new SceneObjects();
    }

    return _eInstance;
}

void collections::SceneObjects::add(const std::string &sObjectName)
{
    objectsIndex++;
    this->_sceneInstancedObjects[objectsIndex] = new scene::SceneObject(objectsIndex, sObjectName);
}

void collections::SceneObjects::remove(const unsigned int &objectID)
{
    // Cancel if a object with this ID doesn't exist
    if (this->_sceneInstancedObjects.find(objectID) == this->_sceneInstancedObjects.end()) { return; }

    delete this->_sceneInstancedObjects[objectID];
    this->_sceneInstancedObjects.erase(objectID);
}

void collections::SceneObjects::addCamera()
{
    objectsIndex++;
    scene::SceneObject *newObject = new scene::SceneObject(objectsIndex);
    scene::Camera *newCamera = new scene::Camera();
    newObject->addComponent(newCamera);
    this->_sceneInstancedObjects[objectsIndex] = newObject;
}

void collections::SceneObjects::addLight(scene::Light::LightType lightType)
{
}

void collections::SceneObjects::addModel(const std::string &sModelName)
{
}

unsigned int collections::SceneObjects::count()
{
    return this->_sceneInstancedObjects.size();
}

unsigned int collections::SceneObjects::objectsIndex = 0;

SceneObjects *collections::SceneObjects::_eInstance = nullptr;


