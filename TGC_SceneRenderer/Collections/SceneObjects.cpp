#include "SceneObjects.h"
using namespace ECollections;

SceneObjects::SceneObjects(void)
{
    _sceneInstancedObjects = gcnew System::Collections::Hashtable();
}

SceneObjects::~SceneObjects()
{
}

SceneObjects ^SceneObjects::Instance()
{
    if (!_eInstance) {
        _eInstance = gcnew SceneObjects();
    }

    return _eInstance;
}