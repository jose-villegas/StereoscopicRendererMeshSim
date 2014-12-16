#pragma once
#include "../Bases/BaseComponent.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"
namespace Scene {

    class Camera : public Bases::BaseComponent {
        public:

            enum TypeProjection {
                Orthographic,
                Perspective
            };

            float nearClipping;
            float farClipping;
            float fieldOfView;
            float aspectRatio;
            glm::vec4 viewPortRect;
            TypeProjection projectionType;
            Camera(void);
            glm::mat4 getViewMatrix(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &vectorUp) const;
            glm::mat4 getProjectionMatrix() const;
            glm::mat4 getProjectionMatrixFrustum() const;
            void setViewPortRect(const float left, const float right, const float bottom, const float top);
    };
}

