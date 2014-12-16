#pragma once
#include "../bases/BaseObject.h"
#include "../bases/BaseComponent.h"
#include "../types/Transform.h"
#include <vector>
namespace scene {

    class SceneObject {
        public:
            unsigned int ID;
            // Holds the base name and state for the scene object - CANT BE NULL
            bases::BaseObject *base;
            // Holds the scene object components, a scene object can have camera, lights and mesh components mixed together
            std::vector<bases::BaseComponent *> components;
            // Tree Hierachy Structure
            // SceneObject *childObject;
            SceneObject(unsigned int index);
            SceneObject(unsigned int index, const std::string &sObjectName);
            void addComponent(bases::BaseComponent *component);
            void removeComponent(const unsigned int &componentIndex);
    };
}

