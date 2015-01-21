#include "Mesh.h"
#include "..\core\Data.h"
#include "..\utils\ProgressiveMeshes.h"
#include "..\collections\MeshesCollection.h"
using namespace scene;

Mesh::Mesh(void) : polyCount(0), vertexCount(0), meshReductionEnabled(false)
{
    texCollection = collections::TexturesCollection::Instance();
    this->base = new bases::BaseObject("Mesh");
    this->meshReductor = nullptr;
}

Mesh::~Mesh(void)
{
    for (auto it = materials.begin(); it != materials.end(); it++) {
        delete *it;
    }

    materials.clear();

    for (auto it = meshEntries.begin(); it != meshEntries.end(); it++) {
        delete *it;
    }

    meshEntries.clear();

    if (this->meshReductionEnabled) { delete this->meshReductor; }

    collections::MeshesCollection::Instance()->removeMesh(this);
}

bool Mesh::loadMesh(const std::string &sFileName)
{
    this->filepath = sFileName; bool bRtrn = false;
    Assimp::Importer Importer;
    // read filename with assimp importer
    const aiScene *pScene = Importer.ReadFile(sFileName.c_str(), aiProcessPreset_TargetRealtime_Quality);

    if (pScene) {
        std::cout << "Mesh(" << this << ") " << "Loading asset " << sFileName << std::endl;
        this->filename = pScene->mRootNode->mName.C_Str();
        this->fileExtension = this->filename.substr(filename.find_last_of(".") + 1);
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
    // Load associated materials
    bool rtrn = initMaterials(pScene, sFilename);
    glm::vec3 maxPos(-std::numeric_limits<float>::infinity()); glm::vec3 minPos(std::numeric_limits<float>::infinity());

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
        // init submesh entry
        this->meshEntries.push_back(initMesh(i, pScene->mMeshes[i]));
        // set whole mesh scene center min max pos values
        glm::vec3 subMeshMax = this->meshEntries.back()->maxPoint;
        glm::vec3 subMeshMin = this->meshEntries.back()->minPoint;
        maxPos.x = subMeshMax.x > maxPos.x ? subMeshMax.x : maxPos.x;
        maxPos.y = subMeshMax.y > maxPos.y ? subMeshMax.y : maxPos.y;
        maxPos.z = subMeshMax.z > maxPos.z ? subMeshMax.z : maxPos.z;
        minPos.x = subMeshMin.x < minPos.x ? subMeshMin.x : minPos.x;
        minPos.y = subMeshMin.y < minPos.y ? subMeshMin.y : minPos.y;
        minPos.z = subMeshMin.z < minPos.z ? subMeshMin.z : minPos.z;
    }

    this->maxPoint = maxPos;
    this->minPoint = minPos;
    this->midPoint = glm::vec3((maxPos.x + minPos.x) / 2, (maxPos.y + minPos.y) / 2, (maxPos.z + minPos.z) / 2);
    // return boolean value if any error occured during loading
    return rtrn;
}

Mesh::SubMesh *Mesh::initMesh(unsigned int index, const aiMesh *paiMesh)
{
    SubMesh *newSubMesh = new SubMesh();
    newSubMesh->materialIndex = paiMesh->mMaterialIndex;
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    aiVector3D maxPos(-std::numeric_limits<float>::infinity()), minPos(std::numeric_limits<float>::infinity());

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D *pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D *pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D *pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        const aiVector3D *pTangent =  paiMesh->HasTangentsAndBitangents() ? &paiMesh->mTangents[i] : &Zero3D;
        const aiVector3D *pBitangent =  paiMesh->HasTangentsAndBitangents() ? &paiMesh->mBitangents[i] : &Zero3D;
        types::Vertex v(
            glm::vec3(pPos->x, pPos->y, pPos->z),
            glm::vec2(pTexCoord->x, pTexCoord->y),
            glm::vec3(pNormal->x, pNormal->y, pNormal->z),
            glm::vec3(pTangent->x, pTangent->y, pTangent->z),
            glm::vec3(pBitangent->x, pBitangent->y, pBitangent->z)
        );
        v.orthogonalize();
        newSubMesh->vertices.push_back(v);
        // set submesh center min max pos values
        maxPos.x = pPos->x > maxPos.x ? pPos->x : maxPos.x;
        maxPos.y = pPos->y > maxPos.y ? pPos->y : maxPos.y;
        maxPos.z = pPos->z > maxPos.z ? pPos->z : maxPos.z;
        minPos.x = pPos->x < minPos.x ? pPos->x : minPos.x;
        minPos.y = pPos->y < minPos.y ? pPos->y : minPos.y;
        minPos.z = pPos->z < minPos.z ? pPos->z : minPos.z;
        // total mesh values
        this->vertexCount++;
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace &face = paiMesh->mFaces[i];
        // verify triangulation preprocess
        assert(face.mNumIndices == 3);
        // indices info
        newSubMesh->indices.push_back(face.mIndices[0]);
        newSubMesh->indices.push_back(face.mIndices[1]);
        newSubMesh->indices.push_back(face.mIndices[2]);
        // add face info
        newSubMesh->faces.push_back(
            types::Face(
                newSubMesh->vertices[face.mIndices[0]],
                newSubMesh->vertices[face.mIndices[1]],
                newSubMesh->vertices[face.mIndices[2]],
                face.mIndices[0],
                face.mIndices[1],
                face.mIndices[2]
            )
        );
        // total mesh values
        polyCount++;
    }

    // setting meshEntry center and min max bounds
    newSubMesh->maxPoint = glm::vec3(maxPos.x, maxPos.y, maxPos.z);
    newSubMesh->minPoint = glm::vec3(minPos.x, minPos.y, minPos.z);
    newSubMesh->midPoint = glm::vec3((maxPos.x + minPos.x) / 2, (maxPos.y + minPos.y) / 2, (maxPos.z + minPos.z) / 2);
    // setting meshEntry vertex and index buffer data
    newSubMesh->generateBuffers();
    newSubMesh->setBuffersData();
    // return created subMesh
    return newSubMesh;
}

bool Mesh::initMaterials(const aiScene *pScene, const std::string &sFilename)
{
    // Extract the directory part from the file name
    std::string::size_type slashIndex = sFilename.find_last_of("/");
    std::string dir, dirPlusSlash;
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

    dirPlusSlash = dir + slashDir;
    bool foundTextures = false;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial *pMaterial = pScene->mMaterials[i];
        types::Material *currentMat = new types::Material();
        // load all mesh associated textures
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Diffuse, dirPlusSlash, currentMat);
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Specular, dirPlusSlash, currentMat);
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Ambient, dirPlusSlash, currentMat);
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Emissive, dirPlusSlash, currentMat);
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Height, dirPlusSlash, currentMat);
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Normals, dirPlusSlash, currentMat);
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Shininess, dirPlusSlash, currentMat);
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Opacity, dirPlusSlash, currentMat);
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Displacement, dirPlusSlash, currentMat);
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Lightmap, dirPlusSlash, currentMat);
        foundTextures |= loadMeshTexture(pMaterial, types::Texture::TextureType::Reflection, dirPlusSlash, currentMat);

        if (currentMat->textureCount() == 0) {
            currentMat->addTexture(texCollection->getDefaultTexture());
        }

        // default material properties
        currentMat->loadMaterialValues(pMaterial);
        // Guess the shader type based on textures supplied
        currentMat->guessMaterialShader();
        // Save current mat to mesh materials
        materials.push_back(currentMat);
    }

    // create a default material if no materials found
    if (pScene->mNumMaterials <= 0) { materials.push_back(new types::Material()); materials.back()->addTexture(texCollection->getDefaultTexture()); }

    return foundTextures;
}

struct Cmp {
    bool operator()(const std::pair<unsigned int, double> &a, const std::pair<unsigned int, double> &b)
    {
        return a.first < b.first;
    }
};

void Mesh::render()
{
    if (!enableRender) { return; }

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    for (unsigned int i = 0 ; i < meshEntries.size() ; i++) {
        // ignore empty submeshes
        if (!meshEntries[i]->enableRender) { continue; }

        const unsigned int materialIndex = meshEntries[i]->materialIndex;
        // Binds the mesh material textures for shader use and set shaders material
        // uniforms, the material shadeprogram has to be set for the uniforms
        this->materials[materialIndex]->useMaterialShader();
        this->materials[materialIndex]->setUniforms();
        // bind vertex buffer and index buffer data to layout locations
        glBindBuffer(GL_ARRAY_BUFFER, meshEntries[i]->VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), 0);						// Vertex Position
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)12);		// Vertex UVS
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)20);		// Vertex Normals
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)32);		// Vertex Tangents
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)44);		// Vertex Bitangets
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEntries[i]->IB);
        // Draw mesh triangles  with loaded buffer object data
        glDrawElements(GL_TRIANGLES, meshEntries[i]->indicesCount, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}

void scene::Mesh::render(const bool positions, const bool uvs, const bool normals, const bool tangents, const bool bitangents, const bool enableShaders /*= true*/)
{
    if (!enableRender) { return; }

    positions  ? glEnableVertexAttribArray(0) : 0;
    uvs        ? glEnableVertexAttribArray(1) : 0;
    normals    ? glEnableVertexAttribArray(2) : 0;
    tangents   ? glEnableVertexAttribArray(3) : 0;
    bitangents ? glEnableVertexAttribArray(4) : 0;

    for (unsigned int i = 0 ; i < meshEntries.size() ; i++) {
        // ignore empty submeshes
        if (!meshEntries[i]->enableRender) { continue; }

        // set mesh material shader and textures
        if (enableShaders) {
            const unsigned int materialIndex = meshEntries[i]->materialIndex;
            // Binds the mesh material textures for shader use and set shaders material
            // uniforms, the material shadeprogram has to be set for the uniforms
            this->materials[materialIndex]->useMaterialShader();
            this->materials[materialIndex]->setUniforms();
        }

        // bind vertex buffer and index buffer data to layout locations
        glBindBuffer(GL_ARRAY_BUFFER, meshEntries[i]->VB);
        positions  ? glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), 0) : 0;						// Vertex Position
        uvs        ? glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)12) : 0;	// Vertex UVS
        normals    ? glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)20) : 0;	// Vertex Normals
        tangents   ? glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)32) : 0;	// Vertex Tangents
        bitangents ? glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)44) : 0;	// Vertex Bitangets
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEntries[i]->IB);
        // Draw mesh triangles  with loaded buffer object data
        glDrawElements(GL_TRIANGLES, meshEntries[i]->indicesCount, GL_UNSIGNED_INT, 0);
    }

    positions  ? glDisableVertexAttribArray(0) : 0;
    uvs        ? glDisableVertexAttribArray(1) : 0;
    normals    ? glDisableVertexAttribArray(2) : 0;
    tangents   ? glDisableVertexAttribArray(3) : 0;
    bitangents ? glDisableVertexAttribArray(4) : 0;
}

void Mesh::SubMesh::generateBuffers()
{
    glGenBuffers(1, &VB);
    glGenBuffers(1, &IB);
}

Mesh::SubMesh::SubMesh()
{
    this->VB            = core::EngineData::Commoms::INVALID_VALUE;
    this->IB            = core::EngineData::Commoms::INVALID_VALUE;
    this->materialIndex = core::EngineData::Commoms::INVALID_MATERIAL;
    this->indicesCount  = 0;
}

scene::Mesh::SubMesh::SubMesh(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<types::Face> &faces)
{
    this->VB            = core::EngineData::Commoms::INVALID_VALUE;
    this->IB            = core::EngineData::Commoms::INVALID_VALUE;
    this->materialIndex = core::EngineData::Commoms::INVALID_MATERIAL;
    this->vertices      = vertices;
    this->indices       = indices;
    this->faces         = faces;
    this->indicesCount	= indices.size();
    this->generateBuffers();
    this->setBuffersData(vertices, indices);
}

void scene::Mesh::SubMesh::setBuffersData(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    if (this->VB == core::EngineData::Commoms::INVALID_VALUE || this->IB == core::EngineData::Commoms::INVALID_VALUE) { return; }

    if (vertices.empty() || indices.empty()) { this->indicesCount = 0; return; }

    this->indicesCount = indices.size();
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(types::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void scene::Mesh::SubMesh::setBuffersData()
{
    this->setBuffersData(this->vertices, this->indices);
}

Mesh::SubMesh::~SubMesh()
{
    this->vertices.clear();
    this->faces.clear();
    this->indices.clear();

    // delete reserve memory for object buffers
    if (VB != core::EngineData::Commoms::INVALID_VALUE) {
        glDeleteBuffers(1, &VB);
    }

    if (IB != core::EngineData::Commoms::INVALID_VALUE) {
        glDeleteBuffers(1, &IB);
    }
}

void scene::Mesh::enableMeshReduction()
{
    if (this->meshReductionEnabled) { return; }

    this->meshReductor = new utils::MeshReductor();
    meshReductor->load(this);
    this->meshReductionEnabled = true;
}

bool scene::Mesh::loadMeshTexture(const aiMaterial *pMaterial, types::Texture::TextureType textureType, std::string dirPlusSlash, types::Material *currentMat)
{
    int diffuseTextureCount = pMaterial->GetTextureCount((aiTextureType)textureType);
    bool foundTextures = false;

    for (int tIndex = 0; tIndex < diffuseTextureCount; tIndex++) {
        aiString textureFilename;

        if (pMaterial->GetTexture((aiTextureType)textureType, tIndex, &textureFilename) == AI_SUCCESS) {
            std::string fullPath = dirPlusSlash + textureFilename.data;
            // verify file extension, assimp loads wavefront .obj bump maps as height maps
            textureType = this->fileExtension == "obj" && textureType == types::Texture::Height ? types::Texture::Normals : textureType;
            // reserve space and create new texture
            types::Texture *newTex = texCollection->addTexture(fullPath, textureType);

            // assign texture to material
            if (nullptr == newTex) {
                currentMat->addTexture(texCollection->getDefaultTexture(), textureType);
            } else {
                currentMat->addTexture(newTex, textureType);
            }
        }

        foundTextures = true;
    }

    return foundTextures;
}
