#pragma once
#include "..\Scene\Mesh.h"

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
    };
}

