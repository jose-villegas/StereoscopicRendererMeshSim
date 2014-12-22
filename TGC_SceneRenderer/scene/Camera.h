#pragma once
#include "../bases/BaseComponent.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"
#include "../collections/CamerasCollection.h"

namespace collections {
    class CamerasCollection;
}

namespace scene {

    class Camera : public bases::BaseComponent {
        private:
            friend class collections::CamerasCollection;
            float nearClipping;
            float farClipping;
            float fieldOfView;
            float aspectRatio;
            glm::vec4 viewPortRect;
            // Only camerascollection can create cameras
            Camera(void);
            Camera(const Camera &cam);
        public:

            enum TypeProjection {
                Orthographic,
                Perspective
            };

            TypeProjection projectionType;

            glm::mat4 getViewMatrix(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &vectorUp) const;
            glm::mat4 getProjectionTypeMatrix() const;
            glm::mat4 getFrustumMatrix() const;
            glm::mat4 getOrthographicMatrix() const;
            void setViewPortRect(const float &left, const float &right, const float &bottom, const float &top);
            void setProjection(const float &aspectRatio, const float &fieldOfView, const float &nearClipping, const float &farClipping);
    };
}

