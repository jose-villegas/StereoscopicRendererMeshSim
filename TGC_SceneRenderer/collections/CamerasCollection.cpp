#include "CamerasCollection.h"
using namespace collections;

CamerasCollection::CamerasCollection(void)
{
    this->activeCamera = 0;
}

CamerasCollection *collections::CamerasCollection::Instance()
{
    if (!instance) {
        instance = new CamerasCollection();
    }

    return instance;
}

scene::Camera *collections::CamerasCollection::createCamera()
{
    this->cameras.push_back(new scene::Camera());
    return this->cameras.back();
}

scene::Camera *collections::CamerasCollection::getCamera(const unsigned int &index)
{
    if (index >= this->cameras.size()) { return nullptr; }

    return this->cameras[index];
}

void collections::CamerasCollection::removeCamera(const unsigned int &index)
{
    if (index >= this->cameras.size()) { return; }

    this->cameras.erase(this->cameras.begin() + index);
}

void collections::CamerasCollection::removeCamera(scene::Camera *cam)
{
    auto it = std::find(this->cameras.begin(), this->cameras.end(), cam);

    if (it == this->cameras.end()) { return; }

    this->cameras.erase(it);
}

void collections::CamerasCollection::setActiveCamera(const unsigned int &index)
{
    if (index >= this->cameras.size()) { return; }

    this->activeCamera = index;
}

scene::Camera *collections::CamerasCollection::getActiveCamera()
{
    if (this->cameras.empty()) { return nullptr; }

    return this->cameras[this->activeCamera];
}

CamerasCollection *collections::CamerasCollection::instance;
