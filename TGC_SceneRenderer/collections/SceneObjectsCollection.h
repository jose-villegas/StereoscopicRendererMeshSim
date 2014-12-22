#pragma once
#include "../scene/SceneObject.h"
#include "../scene/Camera.h"
#include "../scene/Light.h"
#include "../Scene/Mesh.h"
#include "../core/Data.h"
#include "LightsCollection.h"
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
            void addCamera();
            void addLight(scene::Light::LightType lightType);
            void addMesh(const std::string &sMeshname);
            void addMesh(const core::StoredMeshes::Meshes meshId);
            void addMeshFromFile(const std::string &sMeshFilename);
            unsigned int count();
            SceneObjectsCollection(void);
            ~SceneObjectsCollection();

        private:
            // Unique Static Instance
            static SceneObjectsCollection *_instance;
            std::unordered_map<unsigned int, scene::SceneObject *> _sceneObjects;
    };

}
