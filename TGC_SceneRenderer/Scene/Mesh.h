#pragma once
#include "../Types/Vertex.h"
#include "../Bases/BaseComponent.h"
#include "../Collections/Textures.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#include "GLM/glm.hpp"
#include <vector>
#include <iostream>
#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_VALUE 0xFFFFFFFF
namespace Scene {

    class Mesh : public Bases::BaseComponent {
        public:
            Mesh(void);
            ~Mesh(void);
            bool loadMesh(const std::string &sFileName);
            std::vector<unsigned int> textures() const { return _sceneTextures; };
            void render() const;

        private:
            bool initFromScene(const aiScene *paiScene, const std::string &sFilename);
            void initMesh(unsigned int index, const aiMesh *paiMesh);
            bool initMaterials(const aiScene *paiScene, const std::string &sFilename);
            void clear();

            struct MeshEntry {
                GLuint VB;
                GLuint IB;
                unsigned int numIndices;
                unsigned int materialIndex;
                MeshEntry();
                ~MeshEntry();
                void init(const std::vector<Types::Vertex> &vertices, const std::vector<unsigned int> &indices);
            };

            struct Material {
                unsigned int materialIndex;
                std::vector<unsigned int> textureIndices;
                Material(const unsigned int &index);
            };

            std::vector<MeshEntry> _meshEntries;
            std::vector<unsigned int> _sceneTextures;
            std::vector<Material *> _sceneMaterials;
            ECollections::Textures *_texCollection;
    };
}