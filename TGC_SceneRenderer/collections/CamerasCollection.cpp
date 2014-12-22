#include "CamerasCollection.h"
using namespace collections;

CamerasCollection::CamerasCollection(void)
{
}

CamerasCollection *collections::CamerasCollection::Instance()
{
    if (!_instance) { return new CamerasCollection(); }

    return _instance;
}

scene::Camera *collections::CamerasCollection::createCamera()
{
    this->_cameras.push_back(new scene::Camera());
    return this->_cameras.back();
}

scene::Camera *collections::CamerasCollection::getCamera(const unsigned int &index)
{
    if (index > this->_cameras.size()) { return nullptr; }

    return this->_cameras[index];
}

void collections::CamerasCollection::removeCamera(const unsigned int &index)
{
    if (index > this->_cameras.size()) { return; }

    delete this->_cameras[index];
    this->_cameras.erase(this->_cameras.begin() + index);
}

CamerasCollection *collections::CamerasCollection::_instance;
