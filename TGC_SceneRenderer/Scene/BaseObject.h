#pragma once
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\quaternion.hpp"
#include <string>
namespace Scene {

    class BaseObject {
        public:
            std::string objectName;
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;
            BaseObject(void);
            glm::mat4 getModelMatrix();
    };
}

