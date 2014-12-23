#pragma once
#include "../scene/SceneObject.h"
#include "../core/Data.h"
#include "../Scene/Light.h"
#include "../Scene/Camera.h"
#include "../Scene/Mesh.h"
#include <unordered_map>

namespace collections {

    class SceneObjectsCollection {
        private:
            static unsigned int objectsIndex;
        public:
            // Creates Unique Static Instace
            static SceneObjectsCollection *Instance();
            void add(const std::string &sObjectName);
            void remove(const unsigned int &objectID);
            // Default Scene Objects
            scene::Camera *addCamera();
            scene::Light *addLight(scene::Light::LightType lightType);
            scene::Mesh *addMesh(const std::string &sMeshname);
            scene::Mesh *addMesh(const core::StoredMeshes::Meshes meshId);
            scene::Mesh *addMeshFromFile(const std::string &sMeshFilename);
            scene::SceneObject *getSceneObject(const unsigned int &index);
            const std::unordered_map<unsigned int, scene::SceneObject *> &getSceneObjects() const { return _sceneObjects; }
            unsigned int sceneObjectsCount();
            const unsigned int getLastObjectIndex() const;
            SceneObjectsCollection(void);
            ~SceneObjectsCollection();

        private:
            // Unique Static Instance
            static SceneObjectsCollection *_instance;
            std::unordered_map<unsigned int, scene::SceneObject *> _sceneObjects;
    };

}
