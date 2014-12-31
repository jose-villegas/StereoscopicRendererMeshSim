#include "Mesh.h"
#include "..\core\Data.h"
#include "..\utils\ProgressiveMeshes.h"
#include "..\collections\MeshesCollection.h"
using namespace scene;

Mesh::Mesh(void)
{
    texCollection = collections::TexturesCollection::Instance();
    this->base = new bases::BaseObject("Mesh");
}

Mesh::~Mesh(void)
{
    clear();
    collections::MeshesCollection::Instance()->removeMesh(this);
}

bool Mesh::loadMesh(const std::string &sFileName)
{
    // Release the previously loaded mesh (if it exists)
    clear();
    bool bRtrn = false;
    Assimp::Importer Importer;
    // read filename with assimp importer
    const aiScene *pScene = Importer.ReadFile(sFileName.c_str(),
                            aiProcess_Triangulate
                            | aiProcess_JoinIdenticalVertices
                            | aiProcess_GenSmoothNormals
                            | aiProcess_CalcTangentSpace
                            | aiProcess_FlipUVs);

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
    meshEntries.resize(pScene->mNumMeshes);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < meshEntries.size() ; i++) {
        const aiMesh *paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }

    // Load associated materials
    return initMaterials(pScene, sFilename);
}

void Mesh::initMesh(unsigned int index, const aiMesh *paiMesh)
{
    meshEntries[index] = new MeshEntry();
    meshEntries[index]->materialIndex = paiMesh->mMaterialIndex;
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

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
        meshEntries[index]->vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace &face = paiMesh->mFaces[i];
        // verify triangulation preprocess
        assert(face.mNumIndices == 3);
        // indices info
        meshEntries[index]->indices.push_back(face.mIndices[0]);
        meshEntries[index]->indices.push_back(face.mIndices[1]);
        meshEntries[index]->indices.push_back(face.mIndices[2]);
        // add face info
        meshEntries[index]->faces.push_back(
            types::Face(
                meshEntries[index]->vertices[face.mIndices[0]],
                meshEntries[index]->vertices[face.mIndices[1]],
                meshEntries[index]->vertices[face.mIndices[2]],
                face.mIndices[0],
                face.mIndices[1],
                face.mIndices[2]
            )
        );
    }

    utils::ProgressiveMeshes progMesh;
    progMesh.generateProgressiveMesh(meshEntries[index]->vertices, meshEntries[index]->faces);
    progMesh.permuteVertices(meshEntries[index]->vertices, meshEntries[index]->indices, meshEntries[index]->faces);
    utils::ProgressiveMeshes::ReducedMesh *res = progMesh.reorderVertices(meshEntries[index]->vertices, meshEntries[index]->indices, meshEntries[index]->faces, meshEntries[index]->vertices.size() - 1);
    meshEntries[index]->faces = res->faces;
    meshEntries[index]->indices = res->indices;
    meshEntries[index]->vertices = res->vertices;
    meshEntries[index]->generateBuffers();
    meshEntries[index]->setBuffersData();
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
        types::Material *currentMat = new types::Material();
        // Get Texture Count
        int diffuseTextureCount      = pMaterial->GetTextureCount(aiTextureType_DIFFUSE);
        int specularTextureCount     = pMaterial->GetTextureCount(aiTextureType_SPECULAR);
        int ambientextureCount       = pMaterial->GetTextureCount(aiTextureType_AMBIENT);
        int emissiveTextureCount     = pMaterial->GetTextureCount(aiTextureType_EMISSIVE);
        int heightTextureCount       = pMaterial->GetTextureCount(aiTextureType_HEIGHT);
        int normalTextureCount       = pMaterial->GetTextureCount(aiTextureType_NORMALS);
        int shininessTextureCount    = pMaterial->GetTextureCount(aiTextureType_SHININESS);
        int ocapacityTextureCount    = pMaterial->GetTextureCount(aiTextureType_OPACITY);
        int displacementTextureCount = pMaterial->GetTextureCount(aiTextureType_DISPLACEMENT);
        int lightmapTextureCount     = pMaterial->GetTextureCount(aiTextureType_LIGHTMAP);
        int reflectionTextureCount   = pMaterial->GetTextureCount(aiTextureType_REFLECTION);

        for (int tIndex = 0; tIndex < diffuseTextureCount; tIndex++) {
            aiString textureFilename;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, tIndex, &textureFilename) == AI_SUCCESS) {
                std::string fullPath = dir + slashDir + textureFilename.data;

                if (!texCollection->addTexture(fullPath, texCollection->textureCount(), types::Texture::Diffuse)) {
                    bRtrn = false;
                    currentMat->addTexture(texCollection->getDefaultTexture());
                } else {
                    currentMat->addTexture(texCollection->getTexture(texCollection->textureCount() - 1));
                }
            }
        }

        for (int tIndex = 0; tIndex < heightTextureCount; tIndex++) {
            aiString textureFilename;

            if (pMaterial->GetTexture(aiTextureType_HEIGHT, tIndex, &textureFilename) == AI_SUCCESS) {
                std::string fullPath = dir + slashDir + textureFilename.data;

                if (!texCollection->addTexture(fullPath, texCollection->textureCount(), types::Texture::Height)) {
                    bRtrn = false;
                    currentMat->addTexture(texCollection->getDefaultTexture());
                } else {
                    currentMat->addTexture(texCollection->getTexture(texCollection->textureCount() - 1));
                }
            }
        }

        for (int tIndex = 0; tIndex < normalTextureCount; tIndex++) {
            aiString textureFilename;

            if (pMaterial->GetTexture(aiTextureType_NORMALS, tIndex, &textureFilename) == AI_SUCCESS) {
                std::string fullPath = dir + slashDir + textureFilename.data;

                if (!texCollection->addTexture(fullPath, texCollection->textureCount(), types::Texture::Normals)) {
                    bRtrn = false;
                    currentMat->addTexture(texCollection->getDefaultTexture());
                } else {
                    currentMat->addTexture(texCollection->getTexture(texCollection->textureCount() - 1));
                }
            }
        }

        if (currentMat->textureCount() == 0) { currentMat->addTexture(texCollection->getDefaultTexture()); }

        // default material properties
        aiColor4D ambient(0.1f, 0.1f, 0.1f, 0.1f);
        aiColor4D diffuse(0.0f, 0.0f, 0.0f, 0.0f);
        aiColor4D specular(0.0f, 0.0f, 0.0f, 0.0f);
        // Assume diffuse highlights as default
        int shadingModel = core::StoredShaders::Diffuse;
        float shininess = 16.0f;
        // query current material properties
        aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_AMBIENT, &ambient);
        aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
        aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_SPECULAR, &specular);
        aiGetMaterialFloat(pMaterial, AI_MATKEY_SHININESS, &shininess);
        aiGetMaterialInteger(pMaterial, AI_MATKEY_SHADING_MODEL, &shadingModel);
        // Save properties to current material
        currentMat->ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
        currentMat->diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
        currentMat->specular = glm::vec3(specular.r, specular.g, specular.b);
        // Guess the shader type based on textures supplied
        currentMat->guessMaterialShader();
        // Save current mat to mesh materials
        materials.push_back(currentMat);
    }

    return bRtrn;
}

void Mesh::clear()
{
    for (auto it = materials.begin(); it != materials.end(); it++) {
        delete *it;
    }

    materials.clear();

    for (auto it = meshEntries.begin(); it != meshEntries.end(); it++) {
        delete *it;
    }
}

void Mesh::render()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    for (unsigned int i = 0 ; i < meshEntries.size() ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, meshEntries[i]->VB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), 0);						// Vertex Position
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)12);		// Vertex UVS
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)20);		// Vertex Normals
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)32);		// Vertex Tangents
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(types::Vertex), (const GLvoid *)44);		// Vertex Bitangets
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEntries[i]->IB);
        const unsigned int materialIndex = meshEntries[i]->materialIndex;
        // Binds the mesh material textures for shader use and set shaders material
        // uniforms, the material shadeprogram has to be set for the uniforms
        this->materials[materialIndex]->useMaterialShader();
        this->materials[materialIndex]->bindTextures();
        this->materials[materialIndex]->setUniforms();
        // Draw mesh
        glDrawElements(GL_TRIANGLES, meshEntries[i]->indices.size(), GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}

void Mesh::MeshEntry::generateBuffers()
{
    glGenBuffers(1, &VB);
    glGenBuffers(1, &IB);
}

Mesh::MeshEntry::MeshEntry()
{
    this->VB            = core::EngineData::Commoms::INVALID_VALUE;
    this->IB            = core::EngineData::Commoms::INVALID_VALUE;
    this->materialIndex = core::EngineData::Commoms::INVALID_MATERIAL;
}

scene::Mesh::MeshEntry::MeshEntry(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<types::Face> &faces)
{
    this->VB            = core::EngineData::Commoms::INVALID_VALUE;
    this->IB            = core::EngineData::Commoms::INVALID_VALUE;
    this->materialIndex = core::EngineData::Commoms::INVALID_MATERIAL;
    this->vertices      = vertices;
    this->indices       = indices;
    this->faces         = faces;
    this->generateBuffers();
    this->setBuffersData(vertices, indices);
}

void scene::Mesh::MeshEntry::setBuffersData(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(types::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void scene::Mesh::MeshEntry::setBuffersData()
{
    this->setBuffersData(this->vertices, this->indices);
}

Mesh::MeshEntry::~MeshEntry()
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
