#pragma once
#include "..\types\Plane.h"
namespace types {

    class Frustum {
        public:

            enum {
                Top, Bottom, Left, Right, Near, Far
            };

            Frustum(void);

            types::Plane cameraPlanes[6];

            glm::vec3 nearTopLeft;
            glm::vec3 nearTopRight;
            glm::vec3 nearBottomLeft;
            glm::vec3 nearBottomRight;
            glm::vec3 farTopLeft;
            glm::vec3 farTopRight;
            glm::vec3 farBottomLeft;
            glm::vec3 farBottomRight;

            float nearDistance;
            float farDistance;
            float aspectRatio;
            float fieldOfView;

            float nearWidth;
            float nearHeight;
            float farWidth;
            float farHeight;

            void setCameraProjectionParams(const float fov, const float aspectRatio, const float nearDistance, const float farDistance);
            void setCameraViewParams(const glm::vec3 &position, const glm::vec3 target, const glm::vec3 up);
    };
}

