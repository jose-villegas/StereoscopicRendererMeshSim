#include "SceneObjects.h"

ECollections::SceneObjects *ECollections::SceneObjects::_eInstance;

ECollections::SceneObjects::SceneObjects(void)
{
}

ECollections::SceneObjects::~SceneObjects()
{
    _eInstance = 0;
}

ECollections::SceneObjects *ECollections::SceneObjects::Instance()
{
    if (!_eInstance) {
        _eInstance = new ECollections::SceneObjects();
    }

    return _eInstance;
}


