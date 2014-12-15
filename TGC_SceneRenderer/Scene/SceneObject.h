#pragma once
#include "..\Bases\BaseObject.h"
#include "..\Types\Transform.h"
#include "..\Bases\BaseComponent.h"
namespace Scene {
    class SceneObject {
        public:
            // Holds the base name and state for the scene object - CANT BE NULL
            Bases::BaseObject *objectInstance;
            // Holds the model transform for the scene object - CANT BE NULL
            Types::Transform *objectTransform;
            // Holds the scene object components, a scene object can have camera, lights and mesh components mixed together
            Bases::BaseComponent *objectComponents[];
            SceneObject(void);
    };
}
