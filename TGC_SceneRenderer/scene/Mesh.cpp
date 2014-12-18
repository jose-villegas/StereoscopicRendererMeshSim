#include "Mesh.h"
using namespace scene;

Mesh::Mesh(void)
{
    _texCollection = collections::Textures::Instance();
}

Mesh::~Mesh(void)
{
    clear();
}

bool Mesh::loadMesh(const std::string &sFileName)
{
    // Release the previously loaded mesh (if it exists)
    clear();
    bool bRtrn = false;
    Assimp::Importer Importer;
    const aiScene *pScene = Importer.ReadFile(sFileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (pScene) {
        std::cout << "Mesh(" << this << ") " << "Loading asset " << sFileName << std::endl;
        bRtrn = initFromScene(pScene, sFileName);
    } else {
        std::cout << "Mesh(" << this << ") " << "Error parsing '" << sFileName << "': '" << Importer.GetErrorString() << std::endl;
    }

    if (bRtrn) { std::cout << "Mesh(" << this << ") " << "Asset " << sFileName << " loaded successfully" << std::endl; }
    else { std::cout << "Mesh(" << this << ") " << "An error occured loading asset " << sFileName << std::endl; }

    return bRtrn;
}

bool Mesh::initFromScene(const aiScene *pScene, const std::string &sFilename)
{
    _meshEntries.resize(pScene->mNumMeshes);
    _sceneMaterials.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < _meshEntries.size() ; i++) {
        const aiMesh *paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }

    // Load associated materials
    return initMaterials(pScene, sFilename);
}

void Mesh::initMesh(unsigned int index, const aiMesh *paiMesh)
{
    _meshEntries[index].materialIndex = paiMesh->mMaterialIndex;
    std::vector<types::Vertex> vertices;
    std::vector<unsigned int> indices;
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D *pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D *pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D *pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        const aiVector3D *pTangent = &paiMesh->mTangents[i];
        types::Vertex v(
            glm::vec3(pPos->x, pPos->y, pPos->z),
            glm::vec2(pTexCoord->x, pTexCoord->y),
            glm::vec3(pNormal->x, pNormal->y, pNormal->z),
            glm::vec3(pTangent->x, pTangent->y, pTangent->z)
        );
        vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace &face = paiMesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    _meshEntries[index].init(vertices, indices);
}

bool Mesh::initMaterials(const aiScene *pScene, const std::string &sFilename)
{
    // Extract the directory part from the file name
    std::string::size_type slashIndex = sFilename.find_last_of("/");
    std::string dir;
    std::string slashDir = "/";
    // Try with \ slashes if / didn't work out

    if (slashIndex == std::string::npos) {
        std::string::size_type tempSlashIndex = sFilename.find_last_of("\\");

        if (tempSlashIndex != std::string::npos) {
            slashIndex = tempSlashIndex;
            slashDir = "\\";
        }
    }

    if (slashIndex == 0) {
        dir = slashDir;
    } else {
        dir = sFilename.substr(0, slashIndex);
    }

    bool bRtrn = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial *pMaterial = pScene->mMaterials[i];
        Material *currentMaterial = new Material(i);
        // Get Texture Count
        int diffuseTextureCount = pMaterial->GetTextureCount(aiTextureType_DIFFUSE);
        int specularTextureCount = pMaterial->GetTextureCount(aiTextureType_SPECULAR);
        int ambientextureCount = pMaterial->GetTextureCount(aiTextureType_AMBIENT);
        int emissiveTextureCount = pMaterial->GetTextureCount(aiTextureType_EMISSIVE);
        int heightTextureCount = pMaterial->GetTextureCount(aiTextureType_HEIGHT);
        int normalTextureCount = pMaterial->GetTextureCount(aiTextureType_NORMALS);
        int shininessTextureCount = pMaterial->GetTextureCount(aiTextureType_SHININESS);
        int ocapacityTextureCount = pMaterial->GetTextureCount(aiTextureType_OPACITY);
        int displacementTextureCount = pMaterial->GetTextureCount(aiTextureType_DISPLACEMENT);
        int lightmapTextureCount = pMaterial->GetTextureCount(aiTextureType_LIGHTMAP);
        int reflectionTextureCount = pMaterial->GetTextureCount(aiTextureType_REFLECTION);

        for (int tIndex = 0; tIndex < diffuseTextureCount; tIndex++) {
            aiString textureFilename;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, tIndex, &textureFilename) == AI_SUCCESS) {
                std::string fullPath = dir + slashDir + textureFilename.data;

                if (!_texCollection->loadTexture(fullPath, _texCollection->count(), types::Texture::Diffuse)) {
                    bRtrn = false; currentMaterial->textureIndices.push_back(_texCollection->getDefaultTexture());
                } else {
                    currentMaterial->textureIndices.push_back(_texCollection->count() - 1);
                }
            }
        }

        for (int tIndex = 0; tIndex < heightTextureCount; tIndex++) {
            aiString textureFilename;

            if (pMaterial->GetTexture(aiTextureType_HEIGHT, tIndex, &textureFilename) == AI_SUCCESS) {
                std::string fullPath = dir + slashDir + textureFilename.data;

                if (!_texCollection->loadTexture(fullPath, _texCollection->count(), types::Texture::Height)) {
                    bRtrn = false; currentMaterial->textureIndices.push_back(_texCollection->getDefaultTexture());
                } else {
                    currentMaterial->textureIndices.push_back(_texCollection->count() - 1);
                }
            }
        }

        for (int tIndex = 0; tIndex < normalTextureCount; tIndex++) {
            aiString textureFilename;

            if (pMaterial->GetTexture(aiTextureType_NORMALS, tIndex, &textureFilename) == AI_SUCCESS) {
                std::string fullPath = dir + slashDir + textureFilename.data;

                if (!_texCollection->loadTexture(fullPath, _texCollection->count(), types::Texture::Normals)) {
                    bRtrn = false; currentMaterial->textureIndices.push_back(_texCollection->getDefaultTexture());
                } else {
                    currentMaterial->textureIndices.push_back(_texCollection->count() - 1);
                }
            }
        }

        // Save Texture Values for whole scene
        std::vector<unsigned int>::iterator it;

        for (it = currentMaterial->textureIndices.begin(); it != currentMaterial->textureIndices.end(); it++) {
            _sceneTextures.push_back(*it);
        }

        if (currentMaterial->textureIndices.empty()) { currentMaterial->textureIndices.push_back(_texCollection->getDefaultTexture()); }

        _sceneMaterials[i] = currentMaterial;
    }

    return bRtrn;
}

void Mesh::clear()
{
    for (unsigned int i = 0; i < _sceneMaterials.size(); i++) {
        delete _sceneMaterials[i];
    }

    _sceneMaterials.clear();
    _sceneTextures.clear();
}

void Mesh::render() const
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    for (unsigned int i = 0 ; i < _meshEntries.size() ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, _meshEntries[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), 0);						// Vertex Position
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)12);		// Vertex UVS
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)20);		// Vertex Normal
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)32);		// Vertex Tangent
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshEntries[i].IB);
        const unsigned int materialIndex = _meshEntries[i].materialIndex;

        /*
        if (materialIndex < _sceneTextures.size() && _sceneTextures[materialIndex]) {
            _texCollection->bindTexture(_sceneTextures[materialIndex]);
        }
        */

        for (int texIndex = _sceneMaterials[materialIndex]->textureIndices.size() - 1; texIndex >= 0 ; texIndex--) {
            _texCollection->bindTexture(_sceneMaterials[materialIndex]->textureIndices[texIndex]);
        }

        glDrawElements(GL_TRIANGLES, _meshEntries[i].numIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void Mesh::MeshEntry::init(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    numIndices = indices.size();
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(types::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);
}

Mesh::MeshEntry::MeshEntry()
{
    VB = INVALID_VALUE;
    IB = INVALID_VALUE;
    numIndices  = 0;
    materialIndex = INVALID_MATERIAL;
}

Mesh::MeshEntry::~MeshEntry()
{
    if (VB != INVALID_VALUE) {
        glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_VALUE) {
        glDeleteBuffers(1, &IB);
    }
}

scene::Mesh::Material::Material(const unsigned int &index)
{
    this->materialIndex = index;
}
