#pragma once
#include "..\Scene\SceneObject.h"
#include "..\Scene\Camera.h"
#include "..\Scene\Light.h"
#include <unordered_map>
namespace ECollections {

    class SceneObjects {
        private:
            static unsigned int objectsIndex;
        public:
            // Creates Unique Static Instace
            static SceneObjects *Instance();
            void add(const std::string &sObjectName);
            void remove(const unsigned int &objectID);
            // Default Scene Objects
            void addCamera();
            void addLight(Scene::Light::LightType lightType);
            void addModel(const std::string &sModelName);
            unsigned int count();
            SceneObjects(void);
            ~SceneObjects();

        private:
            // Unique Static Instance
            static SceneObjects *_eInstance;
            std::unordered_map<unsigned int, Scene::SceneObject *> _sceneInstancedObjects;
    };

}
