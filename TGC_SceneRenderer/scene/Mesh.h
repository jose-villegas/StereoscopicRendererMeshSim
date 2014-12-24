#pragma once
#include "../types/Vertex.h"
#include "../bases/BaseComponent.h"
#include "../collections/TexturesCollection.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#include "GLM/glm.hpp"
#include <vector>
#include <iostream>
#include "../Types/Material.h"

namespace collections {
    class MeshesCollection;
}

namespace scene {

    class Mesh : public bases::BaseComponent {
        public:
            bool loadMesh(const std::string &sFileName);
            void render();
            std::vector<types::Material *> materials() const { return _materials; };

            ~Mesh(void);

        protected:
            friend class collections::MeshesCollection;

            class MeshEntry {
                public:
                    // MeshEntry general data
                    unsigned int verticesCount;
                    unsigned int indicesCount;
                    unsigned int facesCount;
                    // OpenGL buffer objects identifiers
                    GLuint VB;
                    GLuint IB;

                    void generateBuffers();
                    void setBuffersData(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices);
                private:
                    friend class scene::Mesh;
                    // only mesh outer class can destroy and create mesh entries and manipulate the material indexes
                    unsigned int materialIndex;

                    MeshEntry();
                    ~MeshEntry();
                    MeshEntry(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices);
            };
            // mesh loading can be redefined by child classes to handle
            // themselves the mesh indices, vertices and faces
            virtual void initMesh(unsigned int index, const aiMesh *paiMesh);

        private:
            std::vector<MeshEntry *> _meshEntries;
            std::vector<types::Material *> _materials;
            // Engine Textures Collection
            collections::TexturesCollection *_texCollection;

            Mesh(void);
            Mesh(const Mesh &mesh);

            bool initFromScene(const aiScene *paiScene, const std::string &sFilename);
            bool initMaterials(const aiScene *paiScene, const std::string &sFilename);
            void clear();
    };
}