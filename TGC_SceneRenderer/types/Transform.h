#pragma once

#define GLM_FORCE_RADIANS

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
            void setPosition(const float &value0, const float &value1, const float &value2);
            void setRotation(const float &value0, const float &value1, const float &value2);
            void setScale(const float &value0, const float &value1, const float &value2);
            void setPosition(const glm::vec3 &position);
            void setRotation(const glm::vec3 &yawPitchRoll);
            void setScale(const glm::vec3 &scale);
            glm::vec3 eulerAngles();
    };
}

