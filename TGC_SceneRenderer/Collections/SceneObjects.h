#pragma once
#include "../scene/SceneObject.h"
#include "../scene/Camera.h"
#include "../scene/Light.h"
#include <unordered_map>
namespace collections {

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
            void addLight(scene::Light::LightType lightType);
            void addModel(const std::string &sModelName);
            unsigned int count();
            SceneObjects(void);
            ~SceneObjects();

        private:
            // Unique Static Instance
            static SceneObjects *_eInstance;
            std::unordered_map<unsigned int, scene::SceneObject *> _sceneInstancedObjects;
    };

}
