#pragma once
#include "glm\vec3.hpp"
namespace bounding {

    class Bounds {
        protected:
            glm::vec3 minPoint;
            glm::vec3 maxPoint;
            glm::vec3 midPoint;
        public:
            const glm::vec3 &getMinPoint() const { return minPoint; };
            const glm::vec3 &getMaxPoint() const { return maxPoint; };
            const glm::vec3 &getMidPoint() const { return midPoint; };

            Bounds(void);
    };
}

