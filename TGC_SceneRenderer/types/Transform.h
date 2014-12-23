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
            void setPosition(const float &value0, const float &value1, const float &value2);
            void setRotation(const float &value0, const float &value1, const float &value2);
            void setScale(const float &value0, const float &value1, const float &value2);
            glm::vec3 eulerAngles();
        private:
            static glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
            static glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp);
            static glm::quat RotateTowards(glm::quat q1, glm::quat q2, float maxAngle);
    };
}

