#pragma once
#include "..\Bases\BaseObject.h"
#include "..\Types\Transform.h"
#include "..\Bases\BaseComponent.h"
#include <vector>
namespace Scene {

    class SceneObject {
        public:
            unsigned int ID;
            // Holds the base name and state for the scene object - CANT BE NULL
            Bases::BaseObject *base;
            // Holds the scene object components, a scene object can have camera, lights and mesh components mixed together
            std::vector<Bases::BaseComponent *> components;
            // Tree Hierachy Structure
            // SceneObject *childObject;
            SceneObject(unsigned int index);
            SceneObject(unsigned int index, const std::string &sObjectName);
            void addComponent(Bases::BaseComponent *component);
            void removeComponent(const unsigned int &componentIndex);
    };
}

