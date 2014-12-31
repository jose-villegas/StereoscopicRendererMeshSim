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

namespace scene {

    class Mesh : public bases::BaseComponent {
        public:
            bool loadMesh(const std::string &sFileName);
            void render();
            std::vector<types::Material *> getMaterials() const { return materials; };

            ~Mesh(void);

        protected:
            friend class collections::MeshesCollection;

            class MeshEntry {
                public:
                    // MeshEntry general data
                    std::vector<types::Vertex> vertices;
                    std::vector<unsigned int> indices;
                    std::vector<types::Face> faces;
                    // OpenGL buffer objects identifiers
                    GLuint VB;
                    GLuint IB;

                    void generateBuffers();
                    void setBuffersData(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices);
                    // uses class stored vertices and indexes
                    void setBuffersData();
                private:
                    friend class scene::Mesh;
                    // only mesh outer class can destroy and create mesh entries and manipulate the material indexes
                    unsigned int materialIndex;

                    MeshEntry();
                    ~MeshEntry();
                    MeshEntry(const std::vector<types::Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<types::Face> &faces);
            };
            // mesh loading can be redefined by child classes to handle
            // themselves the mesh indices, vertices and faces
            void initMesh(unsigned int index, const aiMesh *paiMesh);

        private:
            std::vector<MeshEntry *> meshEntries;
            std::vector<types::Material *> materials;
            // Engine Textures Collection
            collections::TexturesCollection *texCollection;

            Mesh(void);
            Mesh(const Mesh &mesh);

            bool initFromScene(const aiScene *paiScene, const std::string &sFilename);
            bool initMaterials(const aiScene *paiScene, const std::string &sFilename);
            void clear();
    };
}