#pragma once
#include "../bases/BaseComponent.h"
#include "../bounding/Bounds.h"
#include "../collections/TexturesCollection.h"
#include "../types/Face.h"
#include "../types/Material.h"
#include "../types/Vertex.h"
#include "Assimp/Importer.hpp"
#include "Assimp/postprocess.h"
#include "Assimp/scene.h"
#include "GLM/glm.hpp"
#include <iostream>
#include <vector>

namespace utils {
    class ProgressiveMesh;
    class MeshReductor;
}

namespace scene {

    class Mesh : public bases::BaseComponent, public bounding::Bounds {
        public:
            void render();
            void render(const bool positions, const bool uvs, const bool normals, const bool tangents, const bool bitangents, const bool enableShaders = true);

            unsigned int getPolyCount() const { return polyCount; }
            unsigned int getVertexCount() const { return vertexCount; }
            unsigned int getSubmeshesCount() const { return this->meshEntries.size(); }

            class SubMesh : public bases::BaseComponent, public bounding::Bounds {
                public:
                    // MeshEntry general data
                    std::vector<types::Vertex> vertices;
                    std::vector<unsigned int> indices;
                    std::vector<types::Face> faces;
                    // Rendering params
                    unsigned int indicesCount;
                    //  ogl data setting / handling
                    void generateBuffers();
                    // VB and IB need to set with generateBuffers() changes
                    // indicesCount depending on size of input indices
                    void setBuffersData(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices);
                    // uses class stored vertices and indexes
                    void setBuffersData();
                private:
                    friend class scene::Mesh;
                    // only mesh outer class can destroy and create mesh entries and manipulate the material indexes
                    unsigned int materialIndex;
                    // OpenGL buffer objects identifiers
                    GLuint VB;
                    GLuint IB;

                    SubMesh();
                    ~SubMesh();
                    SubMesh(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<types::Face> &faces);
            };

            Mesh(void);
            ~Mesh(void);

            bool loadMesh(const std::string &sFileName);
            const unsigned int subMeshCount() const { return this->meshEntries.size(); }

        protected:

            Mesh(const Mesh &mesh) : polyCount(0), vertexCount(0), meshReductionEnabled(false) {};
            unsigned int polyCount;
            unsigned int vertexCount;

        private:

            std::string filepath;
            std::string filename;
            std::string fileExtension;
            std::vector<SubMesh * > meshEntries;
            std::vector<types::Material * > materials;
            // Engine Textures Collection
            collections::TexturesCollection *texCollection;

            Mesh::SubMesh *initMesh(unsigned int index, const aiMesh *paiMesh);
            bool initFromScene(const aiScene *paiScene, const std::string &sFilename);
            bool initMaterials(const aiScene *paiScene, const std::string &sFilename);
            bool loadMeshTexture(const aiMaterial *pMaterial, types::Texture::TextureType textureType, std::string dirPlusSlash, types::Material *currentMat);

            // Mesh Reduction properties
        protected:

            bool meshReductionEnabled;
            utils::MeshReductor *meshReductor;

        public:

            void enableMeshReduction();
            utils::MeshReductor *getMeshReductor() const { return meshReductor; }
            bool isMeshReductionEnabled() const { return meshReductionEnabled; }

            const std::vector<SubMesh * > &getMeshEntries() const { return meshEntries; }
    };
}