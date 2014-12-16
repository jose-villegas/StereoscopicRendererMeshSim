#include "SceneObjects.h"
using namespace ECollections;

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

void ECollections::SceneObjects::add(const std::string &sObjectName)
{
    objectsIndex++;
    this->_sceneInstancedObjects[objectsIndex] = new Scene::SceneObject(objectsIndex, sObjectName);
}

void ECollections::SceneObjects::remove(const unsigned int &objectID)
{
    // Cancel if a object with this ID doesn't exist
    if (this->_sceneInstancedObjects.find(objectID) == this->_sceneInstancedObjects.end()) { return; }

    delete this->_sceneInstancedObjects[objectID];
    this->_sceneInstancedObjects.erase(objectID);
}

void ECollections::SceneObjects::addCamera()
{
    objectsIndex++;
    Scene::SceneObject *newObject = new Scene::SceneObject(objectsIndex);
    Scene::Camera *newCamera = new Scene::Camera();
    newObject->addComponent(newCamera);
    this->_sceneInstancedObjects[objectsIndex] = newObject;
}

void ECollections::SceneObjects::addLight(Scene::Light::LightType lightType)
{
}

void ECollections::SceneObjects::addModel(const std::string &sModelName)
{
}

unsigned int ECollections::SceneObjects::count()
{
    return this->_sceneInstancedObjects.size();
}

unsigned int ECollections::SceneObjects::objectsIndex = 0;

SceneObjects *ECollections::SceneObjects::_eInstance = nullptr;


