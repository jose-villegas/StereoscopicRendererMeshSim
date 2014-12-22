#pragma once
#include "..\Scene\Mesh.h"

namespace scene {
    class Mesh;
}

namespace collections {

    class MeshesCollection {
        private:
            std::vector <scene::Mesh *> _meshes;
            static MeshesCollection *_instance;
            MeshesCollection(void);
            MeshesCollection(const MeshesCollection &meshesColl);
        public:
            static MeshesCollection *Instance();
            scene::Mesh *createMesh();
            scene::Mesh *getMesh(const unsigned int &index);
            void removeMesh(const unsigned int &index);
            void removeMesh(scene::Mesh *mesh);
            unsigned int meshCount() const { return _meshes.size(); }
            std::vector<scene::Mesh *> getMeshes() const { return _meshes; }
    };
}

