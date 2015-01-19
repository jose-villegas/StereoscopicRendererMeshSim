#pragma once
#include "..\Scene\Mesh.h"
#include <utility>

namespace collections {

    class MeshesCollection {
        private:
            static MeshesCollection *instance;
            std::vector <scene::Mesh *> meshes;
            MeshesCollection(void);
            MeshesCollection(const MeshesCollection &meshesColl);

        public:
            ~MeshesCollection();
            static MeshesCollection *Instance();
            scene::Mesh *createMesh(const std::string &sFilename);
            scene::Mesh *createMesh();
            scene::Mesh *getMesh(const unsigned int index);
            void removeMesh(const unsigned int index);
            void removeMesh(scene::Mesh *mesh);
            unsigned int meshCount() const { return meshes.size(); }
            const std::vector<scene::Mesh *> &getMeshes() const { return meshes; }
    };
}

