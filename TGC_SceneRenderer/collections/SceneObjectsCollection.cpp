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
}

SceneObjectsCollection *SceneObjectsCollection::Instance()
{
    if (!_instance) {
        _instance = new SceneObjectsCollection();
    }

    return _instance;
}

void collections::SceneObjectsCollection::add(const std::string &sObjectName)
{
    objectsIndex++;
    scene::SceneObject *newSceneObject = new scene::SceneObject();
    bases::BaseObject *newObject = new bases::BaseObject(sObjectName);
    newSceneObject->setBaseObject(newObject);
    this->_sceneObjects[objectsIndex] = newSceneObject;
}

void collections::SceneObjectsCollection::remove(const unsigned int &objectID)
{
    // Cancel if a object with this ID doesn't exist
    if (this->_sceneObjects.find(objectID) == this->_sceneObjects.end()) { return; }

    // Remove this object also from the scene collectors
    if (typeid(*_sceneObjects[objectID]) == typeid(scene::Camera)) {
        collections::CamerasCollection::Instance()->removeCamera((scene::Camera *)this->_sceneObjects[objectID]);
    } else if (typeid(*_sceneObjects[objectID]) == typeid(scene::Light)) {
        collections::LightsCollection::Instance()->removeLight((scene::Light *)this->_sceneObjects[objectID]);
    } else if (typeid(*_sceneObjects[objectID]) == typeid(scene::Mesh)) {
        collections::MeshesCollection::Instance()->removeMesh((scene::Mesh *)this->_sceneObjects[objectID]);
    }

    this->_sceneObjects.erase(objectID);
}

scene::Camera *collections::SceneObjectsCollection::addCamera()
{
    objectsIndex++;
    scene::SceneObject *newObject = new scene::SceneObject();
    scene::Camera *newCamera = CamerasCollection::Instance()->createCamera();
    newObject->setBaseObject(newCamera->base);
    newObject->addComponent(newCamera);
    this->_sceneObjects[objectsIndex] = newObject;
    return newCamera;
}

scene::Light *collections::SceneObjectsCollection::addLight(scene::Light::LightType lightType)
{
    objectsIndex++;
    scene::SceneObject *newObject = new scene::SceneObject();
    scene::Light *newLight = collections::LightsCollection::Instance()->createLight();
    newObject->setBaseObject(newLight->base);
    newObject->addComponent(newLight);
    this->_sceneObjects[objectsIndex] = newObject;
    return newLight;
}

scene::Mesh *collections::SceneObjectsCollection::addMesh(const std::string &sMeshname)
{
    for (int i = 0; i < core::StoredMeshes::Count; i++) {
        if (sMeshname == std::string(core::StoredMeshes::MESH_NAMES[i])) {
            return addMeshFromFile(core::ExecutionInfo::EXEC_DIR + core::StoredMeshes::MESH_FILENAMES[i]);
        }
    }
}

scene::Mesh *collections::SceneObjectsCollection::addMesh(const core::StoredMeshes::Meshes meshId)
{
    return addMeshFromFile(core::ExecutionInfo::EXEC_DIR + core::StoredMeshes::MESH_FILENAMES[meshId]);
}

unsigned int collections::SceneObjectsCollection::sceneObjectsCount()
{
    return this->_sceneObjects.size();
}

scene::Mesh *collections::SceneObjectsCollection::addMeshFromFile(const std::string &sMeshFilename)
{
    objectsIndex++;
    scene::SceneObject *newObject = new scene::SceneObject();
    scene::Mesh *newMesh = collections::MeshesCollection::Instance()->createMesh();
    newMesh->loadMesh(sMeshFilename);
    newObject->setBaseObject(newMesh->base);
    newObject->addComponent(newMesh);
    this->_sceneObjects[objectsIndex] = newObject;
    return newMesh;
}

unsigned int collections::SceneObjectsCollection::objectsIndex = 0;

SceneObjectsCollection *collections::SceneObjectsCollection::_instance = nullptr;


