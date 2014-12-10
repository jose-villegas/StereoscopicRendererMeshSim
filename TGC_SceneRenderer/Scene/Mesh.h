#pragma once
#include "../Context/OpenGL.h"
#include "../Types/Vertex.h"
#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"
#include "GLM/glm.hpp"
#include <vector>
#define INVALID_MATERIAL 0xFFFFFFFF

class Mesh {
    public:
        Mesh(void);
        ~Mesh(void);
        bool loadMesh(std::string sFileName);

    private:
        bool initFromScene(const aiScene *paiScene, const std::string &sFilename);
        void initMesh(unsigned int index, const aiMesh *paiMesh);
        bool initMaterials(const aiScene *paiScene, const std::string &sFilename);
        void clear();

        struct MeshEntry {
            GLuint VB;
            GLuint IB;
            unsigned int numIndexes;
            unsigned int materialIndex;

            MeshEntry();
            ~MeshEntry();
            void init(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indexes);
        };

        std::vector<MeshEntry> _meshEntries;
        TextureCollection *_texCollection;
};

