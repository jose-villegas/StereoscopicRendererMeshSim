#include "MeshesCollection.h"
using namespace collections;

MeshesCollection::MeshesCollection(void)
{
}

MeshesCollection *collections::MeshesCollection::Instance()
{
    if (!instance) {
        instance = new collections::MeshesCollection();
    }

    return instance;
}

scene::Mesh *collections::MeshesCollection::createMesh()
{
    this->meshes.push_back(new scene::Mesh());
    return this->meshes.back();
}

scene::Mesh *collections::MeshesCollection::getMesh(const unsigned int &index)
{
    if (index >= this->meshes.size()) { return nullptr; }

    return this->meshes[index];
}

void collections::MeshesCollection::removeMesh(const unsigned int &index)
{
    if (index >= this->meshes.size()) { return; }

    delete this->meshes[index];
    this->meshes.erase(this->meshes.begin() + index);
}

void collections::MeshesCollection::removeMesh(scene::Mesh *mesh)
{
    auto it = std::find(this->meshes.begin(), this->meshes.end(), mesh);
    delete *it;
    this->meshes.erase(it);
}

MeshesCollection *collections::MeshesCollection::instance;
