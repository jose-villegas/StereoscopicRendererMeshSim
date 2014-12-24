#include "CamerasCollection.h"
using namespace collections;

CamerasCollection::CamerasCollection(void)
{
    this->_activeCamera = 0;
}

CamerasCollection *collections::CamerasCollection::Instance()
{
    if (!_instance) {
        _instance = new CamerasCollection();
    }

    return _instance;
}

scene::Camera *collections::CamerasCollection::createCamera()
{
    this->_cameras.push_back(new scene::Camera());
    return this->_cameras.back();
}

scene::Camera *collections::CamerasCollection::getCamera(const unsigned int &index)
{
    if (index >= this->_cameras.size()) { return nullptr; }

    return this->_cameras[index];
}

void collections::CamerasCollection::removeCamera(const unsigned int &index)
{
    if (index >= this->_cameras.size()) { return; }

    delete this->_cameras[index];
    this->_cameras.erase(this->_cameras.begin() + index);
}

void collections::CamerasCollection::removeCamera(scene::Camera *cam)
{
    auto it = std::find(this->_cameras.begin(), this->_cameras.end(), cam);
    delete *it;
    this->_cameras.erase(it);
}

void collections::CamerasCollection::setActiveCamera(const unsigned int &index)
{
    if (index >= this->_cameras.size()) { return; }

    this->_activeCamera = index;
}

scene::Camera *collections::CamerasCollection::getActiveCamera()
{
    return this->_cameras[this->_activeCamera];
}

CamerasCollection *collections::CamerasCollection::_instance;
