#pragma once
#include "glm\detail\type_vec3.hpp"
namespace types {

    class Plane {
        public:
            // defined by a triangle
            glm::vec3 points[3];
            // or defined by a point and a normal
            glm::vec3 normal;
            glm::vec3 point;
            // misc param
            float planeDistance;

            void setPlane(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);
            void setPlane(const glm::vec3 &position, const glm::vec3 &normal);

            float distance(glm::vec3 position);

            Plane(void);
            ~Plane(void);
    };
}

