#include "SceneObjectsCollection.h"
#include "../scene/Camera.h"
#include "../scene/Light.h"
#include "../Scene/Mesh.h"
#include "MeshesCollection.h"
using namespace collections;

SceneObjectsCollection::SceneObjectsCollection(void)
{
}

SceneObjectsCollection::~SceneObjectsCollection()
{
    while (!this->sceneObjects.empty()) {
        delete(*this->sceneObjects.begin()).second;
        this->sceneObjects.erase(this->sceneObjects.begin());
    }
}

SceneObjectsCollection *SceneObjectsCollection::Instance()
{
    if (!instance) {
        instance = new SceneObjectsCollection();
    }

    return instance;
}

void collections::SceneObjectsCollection::add(const std::string &sObjectName)
{
    objectsIndex++;
    scene::SceneObject *newSceneObject = new scene::SceneObject();
    bases::BaseObject *newObject = new bases::BaseObject(sObjectName);
    newSceneObject->setBaseObject(newObject);
    this->sceneObjects[objectsIndex] = newSceneObject;
}

void collections::SceneObjectsCollection::remove(const unsigned int &objectID)
{
    // Cancel if a object with this ID doesn't exist
    if (this->sceneObjects.find(objectID) == this->sceneObjects.end()) { return; }

    delete sceneObjects[objectID];
    this->sceneObjects.erase(objectID);
}

scene::Camera *collections::SceneObjectsCollection::addCamera()
{
    objectsIndex++;
    scene::SceneObject *newObject = new scene::SceneObject();
    scene::Camera *newCamera = CamerasCollection::Instance()->createCamera();
    newObject->setBaseObject(newCamera->base);
    newObject->addComponent(newCamera);
    this->sceneObjects[objectsIndex] = newObject;
    return newCamera;
}

scene::Light *collections::SceneObjectsCollection::addLight(scene::Light::LightType lightType)
{
    objectsIndex++;
    scene::SceneObject *newObject = new scene::SceneObject();
    scene::Light *newLight = collections::LightsCollection::Instance()->createLight();
    newObject->setBaseObject(newLight->base);
    newObject->addComponent(newLight);
    this->sceneObjects[objectsIndex] = newObject;
    return newLight;
}

scene::Mesh *collections::SceneObjectsCollection::addMesh(const std::string &sMeshname)
{
    for (int i = 0; i < core::StoredMeshes::Count; i++) {
        if (sMeshname == std::string(core::StoredMeshes::NAMES[i])) {
            scene::Mesh *newMesh = addMeshFromFile(core::StoredMeshes::Filename(i));
            newMesh->base->objectName = sMeshname;
            return newMesh;
        }
    }

    return nullptr;
}

scene::Mesh *collections::SceneObjectsCollection::addMesh(const core::StoredMeshes::Meshes meshId)
{
    scene::Mesh *newMesh = addMeshFromFile(core::StoredMeshes::Filename(meshId));
    newMesh->base->objectName = core::StoredMeshes::NAMES[meshId];
    return newMesh;
}

unsigned int collections::SceneObjectsCollection::sceneObjectsCount()
{
    return this->sceneObjects.size();
}

scene::Mesh *collections::SceneObjectsCollection::addMeshFromFile(const std::string &sMeshFilename)
{
    objectsIndex++;
    scene::SceneObject *newObject = new scene::SceneObject();
    scene::Mesh *newMesh = collections::MeshesCollection::Instance()->createMesh();
    newMesh->loadMesh(sMeshFilename);
    newObject->setBaseObject(newMesh->base);
    newObject->addComponent(newMesh);
    this->sceneObjects[objectsIndex] = newObject;
    return newMesh;
}

scene::SceneObject *collections::SceneObjectsCollection::getSceneObject(const unsigned int &index)
{
    if (this->sceneObjects.find(index) == this->sceneObjects.end()) { return nullptr; }

    return this->sceneObjects[index];
}

const unsigned int collections::SceneObjectsCollection::getLastObjectIndex() const
{
    return objectsIndex;
}

unsigned int collections::SceneObjectsCollection::objectsIndex = 0;

SceneObjectsCollection *collections::SceneObjectsCollection::instance = nullptr;


