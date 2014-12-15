#include "Camera.h"
using namespace Scene;

Camera::Camera(void)
{
}

glm::mat4 Scene::Camera::getViewMatrix(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &vectorUp) const
{
    return glm::lookAt(cameraPosition, cameraTarget, vectorUp);
}

glm::mat4 Scene::Camera::getProjectionMatrix() const
{
    if (this->projectionType == Perspective) {
        return glm::perspective(this->fieldOfView, this->aspectRatio, this->nearClipping, this->farClipping);
    } else {
        return glm::ortho(this->viewPortRect.x, this->viewPortRect.y, this->viewPortRect.z, this->viewPortRect.w, this->nearClipping, this->farClipping);
    }
}

glm::mat4 Scene::Camera::getProjectionMatrixFrustum() const
{
    return glm::frustum(this->viewPortRect.x, this->viewPortRect.y, this->viewPortRect.z, this->viewPortRect.w, this->nearClipping, this->farClipping);
}

void Scene::Camera::setViewPortRect(const float left, const float right, const float bottom, const float top)
{
    this->viewPortRect = glm::vec4(left, right, bottom, top);
}
