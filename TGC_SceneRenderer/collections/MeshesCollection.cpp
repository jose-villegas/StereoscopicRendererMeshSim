#include "MeshesCollection.h"
using namespace collections;

MeshesCollection::MeshesCollection(void)
{
}

MeshesCollection *collections::MeshesCollection::Instance()
{
    if (!_instance) { return new collections::MeshesCollection(); }

    return _instance;
}

scene::Mesh *collections::MeshesCollection::createMesh()
{
    this->_meshes.push_back(new scene::Mesh());
    return this->_meshes.back();
}

scene::Mesh *collections::MeshesCollection::getMesh(const unsigned int &index)
{
    if (index > this->_meshes.size()) { return nullptr; }

    return this->_meshes[index];
}

void collections::MeshesCollection::removeMesh(const unsigned int &index)
{
    if (index > this->_meshes.size()) { return; }

    delete this->_meshes[index];
    this->_meshes.erase(this->_meshes.begin() + index);
}

void collections::MeshesCollection::removeMesh(scene::Mesh *mesh)
{
    auto it = std::find(this->_meshes.begin(), this->_meshes.end(), mesh);
    this->_meshes.erase(it);
}

MeshesCollection *collections::MeshesCollection::_instance;
