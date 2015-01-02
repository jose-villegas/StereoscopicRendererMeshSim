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

scene::Mesh *collections::MeshesCollection::createMesh(const std::string &sFilename)
{
    this->meshes.push_back(new scene::Mesh());
    this->meshes.back()->loadMesh(sFilename);
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

    this->meshes.erase(this->meshes.begin() + index);
}

void collections::MeshesCollection::removeMesh(scene::Mesh *mesh)
{
    auto it = std::find(this->meshes.begin(), this->meshes.end(), mesh);

    if (it == this->meshes.end()) { return; }

    this->meshes.erase(it);
}

collections::MeshesCollection::~MeshesCollection()
{
    this->meshes.clear();
}

MeshesCollection *collections::MeshesCollection::instance;
