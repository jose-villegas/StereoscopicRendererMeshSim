#include "Mesh.h"

Mesh::Mesh(void)
{
    _texCollection->Inst();
}

Mesh::~Mesh(void)
{
}

bool Mesh::loadMesh(std::string sFileName)
{
    return true;
}

bool Mesh::initFromScene(const aiScene *pScene, const std::string &Filename)
{
    return true;
}

void Mesh::initMesh(unsigned int Index, const aiMesh *paiMesh)
{
}

bool Mesh::initMaterials(const aiScene *pScene, const std::string &Filename)
{
    return true;
}

void Mesh::clear()
{
}

void Mesh::MeshEntry::init(const std::vector<Vertex> &Vertices, const std::vector<unsigned int> &Indices)
{
}

Mesh::MeshEntry::MeshEntry()
{
}

Mesh::MeshEntry::~MeshEntry()
{
}
