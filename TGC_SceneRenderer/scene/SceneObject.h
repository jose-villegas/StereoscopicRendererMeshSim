#pragma once
#include "../bases/BaseObject.h"
#include "../bases/BaseComponent.h"
#include "../types/Transform.h"
#include <vector>
namespace scene {

    class SceneObject {
        private:
            // Holds the base name and state for the scene object - CANT BE NULL
            bases::BaseObject *base;
            // Holds the scene object components, a scene object can have camera, lights and mesh components mixed together
            std::vector<bases::BaseComponent *> components;
        public:

            // Tree Hierachy Structure
            // SceneObject *childObject;

            SceneObject();
            void addComponent(bases::BaseComponent *component);
            void removeComponent(const unsigned int &componentIndex);
            void setBaseObject(bases::BaseObject *base);
    };
}

