#pragma once
#include "GLM/glm.hpp"
#include "GLM/gtx/transform.hpp"
#include "GLM/gtc/quaternion.hpp"
namespace types {

    class Transform {
        public:
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;
            Transform(void);
            glm::mat4 getModelMatrix();
    };
}

