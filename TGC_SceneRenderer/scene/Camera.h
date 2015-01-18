#pragma once

#define GLM_FORCE_RADIANS

#include "../bases/BaseComponent.h"
#include "../collections/CamerasCollection.h"
#include "../types/Frustum.h"
#include "../types/Plane.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace collections {
    class CamerasCollection;
}

namespace core {
    class Engine;
}

namespace scene {

    class Camera : public bases::BaseComponent {
        protected:
            friend class collections::CamerasCollection;

            types::Frustum cameraFrustum;
            bool enableFrustumCulling;
            // Depend on each other so they need to be encapsulated anyways
            float nearClippingPlane;
            float farClippingPlane;
            glm::vec4 horizontalVerticalClipping; // left - right - bottom - top
            glm::vec3 vectorUp;
            float fieldOfView;
            float aspectRatio;
            float width;
            float height;
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

            glm::vec3 getCameraTarget() const;
            void renderMeshes(const core::Engine *actRenderer);

        public:

            enum TypeProjection {
                Perspective,
                Orthographic,
                Stereoscopic // anaglyph stereo 3d projection
            };

            TypeProjection projectionType;

            glm::mat4 getViewMatrix(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &vectorUp) const;
            glm::mat4 getViewMatrix();
            glm::mat4 getProjectionTypeMatrix() const;
            glm::mat4 getFrustumMatrix() const;
            glm::mat4 getOrthographicMatrix() const;
            // generalized setters
            void setViewPortRect(const float &left, const float &right, const float &bottom, const float &top);
            void setProjection(const float &aspectRatio, const float &fieldOfView, const float &nearClipping, const float &farClipping);
            // members getters
            float getNearClippingPlane() const { return this->nearClippingPlane; }
            float getFarClippingPlane() const { return this->farClippingPlane; }
            const glm::vec3 &calculateVectorUp();
            const glm::vec3 &getVectorUp() const { return vectorUp ;}
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
            void setVectorUp(float a, float b, float c) { vectorUp = glm::vec3(a, b, c); }
            // renders scene meshes from the camera point of view
            void renderFromPOV(const core::Engine *actRenderer);
            // sets the rendering view port and updates camera accordly,
            // warning this changes the whole render target viewport
            void viewport(const unsigned int width, const unsigned int height);
    };
}

