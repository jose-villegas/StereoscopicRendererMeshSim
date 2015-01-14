#pragma once
#include "../types/Vertex.h"
#include "../types/Face.h"
#include "../types/Material.h"
#include "../bases/BaseComponent.h"
#include "../collections/TexturesCollection.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#include "GLM/glm.hpp"
#include <vector>
#include <iostream>

namespace collections {
    class MeshesCollection;
}

namespace utils {
    class ProgressiveMesh;
    class MeshReductor;
}

namespace scene {

    class Mesh : public bases::BaseComponent {
        public:
            void render();

            unsigned int getPolyCount() const { return polyCount; }
            unsigned int getVertexCount() const { return vertexCount; }
            unsigned int getSubmeshesCount() const { return this->meshEntries.size(); }

            class SubMesh {
                public:
                    // MeshEntry general data
                    std::vector<types::Vertex> vertices;
                    std::vector<unsigned int> indices;
                    std::vector<types::Face> faces;
                    glm::vec3 centerVertex;
                    glm::vec3 minVertex;
                    glm::vec3 maxVertex;
                    // Rendering params
                    unsigned int indicesCount;
                    bool enableRendering;
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

        protected:
            friend class collections::MeshesCollection;

            unsigned int polyCount;
            unsigned int vertexCount;
            bool enableRendering;

        private:

            std::string filepath;
            std::string filename;
            std::string fileExtension;
            std::vector<SubMesh * > meshEntries;
            std::vector<types::Material * > materials;
            // Engine Textures Collection
            collections::TexturesCollection *texCollection;

            Mesh(void);
            Mesh(const Mesh &mesh) : polyCount(0), vertexCount(0), enableRendering(true), meshReductionEnabled(false) {};
            ~Mesh(void);

            bool loadMesh(const std::string &sFileName);
            void initMesh(unsigned int index, const aiMesh *paiMesh);
            bool initFromScene(const aiScene *paiScene, const std::string &sFilename);
            bool initMaterials(const aiScene *paiScene, const std::string &sFilename);

            bool loadMeshTexture(const aiMaterial *pMaterial, types::Texture::TextureType textureType, std::string dirPlusSlash, types::Material *currentMat);

            const unsigned int subMeshCount() const { return this->meshEntries.size(); }

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