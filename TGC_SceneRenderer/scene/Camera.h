#pragma once

#define GLM_FORCE_RADIANS

#include "../bases/BaseComponent.h"
#include "../collections/CamerasCollection.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace collections {
    class CamerasCollection;
}

namespace core {
    class Renderer;
}

namespace scene {

    class Camera : public bases::BaseComponent {
        private:
            friend class collections::CamerasCollection;
            // control vars
            bool changedValues;
            // planes structure
            struct Plane {
                // left-buttom, right-buttom, left-top, right-top
                glm::vec3 points[4];
                float width;
                float height;
                float distance; // distance from camera position
                Plane() : width(-1), height(-1) {};

                void updatePoints(glm::vec3 position, glm::vec3 direction, glm::vec3 up, glm::vec3 right);
            };

            void renderMeshes(const core::Renderer *actRenderer);
            // Depend on each other so they need to be encapsulated anyways
            Plane nearClippingPlane;
            Plane farClippingPlane;
            glm::vec4 horizontalVerticalClipping; // left - right - bottom - top
            glm::vec3 vectorUp;
            float fieldOfView;
            float aspectRatio;
            // orthographic projection size
            float orthoProjectionSize;
            // stereo 3d projection data members
            float zeroParallax;
            float eyeSeparation;
            glm::mat4 leftFrustum();
            glm::mat4 rightFrustum();
            // Only camerascollection can create cameras
            Camera(void);
            Camera(const Camera &cam);
            ~Camera(void);
        public:

            enum TypeProjection {
                Perspective,
                Orthographic,
                Stereoscopic // anaglyph stereo 3d projection
            };

            TypeProjection projectionType;

            glm::mat4 getViewMatrix(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &vectorUp) const;
            glm::mat4 getViewMatrix() const;
            glm::mat4 getProjectionTypeMatrix() const;
            glm::mat4 getFrustumMatrix() const;
            glm::mat4 getOrthographicMatrix() const;
            // generalized setters
            void setViewPortRect(const float &left, const float &right, const float &bottom, const float &top);
            void setProjection(const float &aspectRatio, const float &fieldOfView, const float &nearClipping, const float &farClipping);
            // members getters
            const Plane &getNearClippingPlane() const { return this->nearClippingPlane; }
            const Plane &getFarClippingPlane() const { return this->farClippingPlane; }
            const glm::vec3 &getVectorUp() const { return vectorUp; }
            float getFieldOfView() const { return fieldOfView; }
            float getAspectRatio() const { return aspectRatio; }
            float getOrthoProjectionSize() const { return orthoProjectionSize; }
            float getZeroParallax() const { return zeroParallax; }
            float getEyeSeparation() const { return eyeSeparation; }
            // members setters, update projection based on new values
            void setAspectRatio(const float val);
            void setNearClippingDistance(const float val);
            void setFarClippingDistance(const float val);
            void setFieldOfView(const float val);
            void setEyeSeparation(const float val);
            void setOrthoProjectionSize(float val) { orthoProjectionSize = val; }
            void setZeroParallax(float val) { zeroParallax = val; }
            void setVectorUp(glm::vec3 val) { vectorUp = val; }
            void setVectorUp(float a, float b, float c) { vectorUp = glm::vec3(a, b, c); }
            // camera render meshes
            void renderFromPOV(const core::Renderer *actRenderer);
    };
}

