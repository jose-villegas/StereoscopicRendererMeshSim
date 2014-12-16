#include "Camera.h"
using namespace scene;

Camera::Camera(void)
{
    this->projectionType = Perspective;
    this->farClipping = 100.0f;
    this->nearClipping = 0.1f;
    this->fieldOfView = 60.0f;
    this->aspectRatio = 4.0f / 3.0f;
    this->viewPortRect = glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
}

glm::mat4 scene::Camera::getViewMatrix(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &vectorUp) const
{
    return glm::lookAt(cameraPosition, cameraTarget, vectorUp);
}

glm::mat4 scene::Camera::getProjectionMatrix() const
{
    if (this->projectionType == Perspective) {
        return glm::perspective(this->fieldOfView, this->aspectRatio, this->nearClipping, this->farClipping);
    } else if (this->projectionType == Orthographic) {
        return glm::ortho(this->viewPortRect.x, this->viewPortRect.y, this->viewPortRect.z, this->viewPortRect.w, this->nearClipping, this->farClipping);
    }

    return glm::mat4(1.0);
}

glm::mat4 scene::Camera::getProjectionMatrixFrustum() const
{
    return glm::frustum(this->viewPortRect.x, this->viewPortRect.y, this->viewPortRect.z, this->viewPortRect.w, this->nearClipping, this->farClipping);
}

void scene::Camera::setViewPortRect(const float left, const float right, const float bottom, const float top)
{
    this->viewPortRect = glm::vec4(left, right, bottom, top);
}
