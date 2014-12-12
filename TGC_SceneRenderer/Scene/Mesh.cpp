#include "Mesh.h"
using namespace Scene;

Mesh::Mesh(void)
{
    _texCollection->Instance();
}

Mesh::~Mesh(void)
{
    clear();
}

bool Mesh::loadMesh(std::string sFileName)
{
    // Release the previously loaded mesh (if it exists)
    clear();
    bool bRtrn = false;
    Assimp::Importer Importer;
    const aiScene *pScene = Importer.ReadFile(sFileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (pScene) {
        bRtrn = initFromScene(pScene, sFileName);
    } else {
        std::string sError =  "Error parsing '" + sFileName + "': '" + Importer.GetErrorString();
        Utils::Logger::Write(gcnew System::String(sError.c_str()), true, System::Drawing::Color::Red);
    }

    return bRtrn;
}

bool Mesh::initFromScene(const aiScene *pScene, const std::string &Filename)
{
    _meshEntries.resize(pScene->mNumMeshes);
    _meshTextures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < _meshEntries.size() ; i++) {
        const aiMesh *paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }

    return initMaterials(pScene, Filename);
}

void Mesh::initMesh(unsigned int Index, const aiMesh *paiMesh)
{
    _meshEntries[Index].materialIndex = paiMesh->mMaterialIndex;
    std::vector<Types::Vertex> Vertices;
    std::vector<unsigned int> Indices;
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D *pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D *pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D *pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        Types::Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z), glm::vec2(pTexCoord->x, pTexCoord->y), glm::vec3(pNormal->x, pNormal->y, pNormal->z));
        Vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace &Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    _meshEntries[Index].init(Vertices, Indices);
}

bool Mesh::initMaterials(const aiScene *pScene, const std::string &Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    } else if (SlashIndex == 0) {
        Dir = "/";
    } else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool bRtrn = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial *pMaterial = pScene->mMaterials[i];
        _meshTextures[i] = INVALID_MATERIAL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath = Dir + "/" + Path.data;

                if (!_texCollection->loadTexture(FullPath.c_str(), _texCollection->count(), GL_RGBA, GL_RGBA, 0, 0)) {
                    std::string sError = "Error loading texture '" + FullPath;
                    Utils::Logger::Write(gcnew System::String(sError.c_str()), true, System::Drawing::Color::Red);
                    bRtrn = false;
                } else {
                    _meshTextures[i] = _texCollection->count() - 1;
                    std::string sSuccess = "Loaded texture '" + FullPath;
                    Utils::Logger::Write(gcnew System::String(sSuccess.c_str()), true, System::Drawing::Color::Green);
                }
            }
        }

        // Load a white texture in case the model does not include its own texture
        if (_meshTextures[i] == INVALID_MATERIAL) {
            _meshTextures[i] = _texCollection->getDefaultTexture();
        }
    }

    return bRtrn;
}

void Mesh::clear()
{
    _meshTextures.clear();
}

void Mesh::render()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0 ; i < _meshEntries.size() ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, _meshEntries[i].VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex), (const GLvoid *)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex), (const GLvoid *)20);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshEntries[i].IB);
        const unsigned int materialIndex = _meshEntries[i].materialIndex;

        if (materialIndex < _meshTextures.size() && _meshTextures[materialIndex]) {
            _texCollection->bindTexture(_meshTextures[materialIndex]);
        }

        glDrawElements(GL_TRIANGLES, _meshEntries[i].numIndexes, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Mesh::MeshEntry::init(const std::vector<Types::Vertex> &vertices, const std::vector<unsigned int> &indexes)
{
    numIndexes = indexes.size();
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Types::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndexes, &indexes[0], GL_STATIC_DRAW);
}

Mesh::MeshEntry::MeshEntry()
{
    VB = INVALID_VALUE;
    IB = INVALID_VALUE;
    numIndexes  = 0;
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
