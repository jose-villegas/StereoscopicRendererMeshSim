#include "SceneObjects.h"
using namespace ECollections;

SceneObjects *SceneObjects::_eInstance;

SceneObjects::SceneObjects(void)
{
}

SceneObjects::~SceneObjects()
{
    _eInstance = 0;
}

SceneObjects *SceneObjects::Instance()
{
    if (!_eInstance) {
        _eInstance = new SceneObjects();
    }

    return _eInstance;
}


