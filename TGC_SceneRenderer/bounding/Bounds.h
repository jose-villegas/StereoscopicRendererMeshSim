#pragma once
#include "glm\vec3.hpp"
namespace bounding {

    class Bounds {
        public:
            glm::vec3 minPoint;
            glm::vec3 maxPoint;
            glm::vec3 midPoint;

            Bounds(void);
    };
}

