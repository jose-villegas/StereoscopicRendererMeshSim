#include "SceneObjectsCollection.h"
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

    delete this->_sceneObjects[objectID];
    this->_sceneObjects.erase(objectID);
}

void collections::SceneObjectsCollection::addCamera()
{
    objectsIndex++;
    scene::SceneObject *newObject = new scene::SceneObject();
    scene::Camera *newCamera = CamerasCollection::Instance()->createCamera();
    newObject->setBaseObject(newCamera->base);
    newObject->addComponent(newCamera);
    this->_sceneObjects[objectsIndex] = newObject;
}

void collections::SceneObjectsCollection::addLight(scene::Light::LightType lightType)
{
    objectsIndex++;
    scene::SceneObject *newObject = new scene::SceneObject();
    scene::Light *newLight = collections::LightsCollection::Instance()->createLight();
    newObject->setBaseObject(newLight->base);
    newObject->addComponent(newLight);
    this->_sceneObjects[objectsIndex] = newObject;
}

void collections::SceneObjectsCollection::addMesh(const std::string &sMeshname)
{
    for (int i = 0; i < core::StoredMeshes::Count; i++) {
        if (sMeshname == std::string(core::StoredMeshes::MESH_NAMES[i])) {
            addMeshFromFile(core::ExecutionInfo::EXEC_DIR + core::StoredMeshes::MESH_FILENAMES[i]);
            break;
        }
    }
}

void collections::SceneObjectsCollection::addMesh(const core::StoredMeshes::Meshes meshId)
{
    addMeshFromFile(core::ExecutionInfo::EXEC_DIR + core::StoredMeshes::MESH_FILENAMES[meshId]);
}

unsigned int collections::SceneObjectsCollection::count()
{
    return this->_sceneObjects.size();
}

void collections::SceneObjectsCollection::addMeshFromFile(const std::string &sMeshFilename)
{
    objectsIndex++;
    scene::SceneObject *newObject = new scene::SceneObject();
    scene::Mesh *newMesh = new scene::Mesh();
    newMesh->loadMesh(sMeshFilename);
}

unsigned int collections::SceneObjectsCollection::objectsIndex = 0;

SceneObjectsCollection *collections::SceneObjectsCollection::_instance = nullptr;


