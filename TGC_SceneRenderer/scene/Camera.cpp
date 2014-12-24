#include "Camera.h"
using namespace scene;

Camera::Camera(void)
{
    this->projectionType = Perspective;
    this->farClipping = 1000.0f;
    this->nearClipping = 0.1f;
    this->fieldOfView = 75.0f;
    this->aspectRatio = 16.0f / 9.0f;
    float ymax = nearClipping * glm::tan(fieldOfView * glm::pi<float>() / 360.0f);
    float xmax = ymax * aspectRatio;
    this->horizontalVerticalClipping = glm::vec4(-xmax, xmax, -ymax, ymax);
    this->base = new bases::BaseObject("Camera");
}

glm::mat4 scene::Camera::getViewMatrix(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &vectorUp) const
{
    return glm::lookAt(cameraPosition, cameraTarget, vectorUp);
}

glm::mat4 scene::Camera::getViewMatrix() const
{
    glm::vec3 cameraPosition = this->base->transform.position;
    glm::vec3 cameraTarget = (this->base->transform.position + glm::vec3(0.0, 0.0, -1.0)) * glm::mat3_cast(this->base->transform.rotation);
    return getViewMatrix(cameraPosition, cameraTarget, glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 scene::Camera::getProjectionTypeMatrix() const
{
    if (this->projectionType == Perspective) {
        return glm::perspective(this->fieldOfView, this->aspectRatio, this->nearClipping, this->farClipping);
    } else if (this->projectionType == Orthographic) {
        return glm::ortho(this->horizontalVerticalClipping.x, this->horizontalVerticalClipping.y, this->horizontalVerticalClipping.z, this->horizontalVerticalClipping.w, this->nearClipping,
                          this->farClipping);
    }

    return glm::mat4(1.0);
}

glm::mat4 scene::Camera::getFrustumMatrix() const
{
    return glm::frustum(this->horizontalVerticalClipping.x, this->horizontalVerticalClipping.y, this->horizontalVerticalClipping.z, this->horizontalVerticalClipping.w, this->nearClipping,
                        this->farClipping);
}

glm::mat4 scene::Camera::getOrthographicMatrix() const
{
    return glm::ortho(this->horizontalVerticalClipping.x, this->horizontalVerticalClipping.y, this->horizontalVerticalClipping.z, this->horizontalVerticalClipping.w, this->nearClipping,
                      this->farClipping);
}

void scene::Camera::setViewPortRect(const float &left, const float &right, const float &bottom, const float &top)
{
    this->horizontalVerticalClipping = glm::vec4(left, right, bottom, top);
}

void scene::Camera::setProjection(const float &aspectRatio, const float &fieldOfView, const float &nearClipping, const float &farClipping)
{
    this->farClipping = farClipping;
    this->nearClipping = nearClipping;
    this->fieldOfView = fieldOfView;
    this->aspectRatio = aspectRatio;
    // Set ViewPort Accordly
    float ymax = nearClipping * glm::tan(fieldOfView * glm::pi<float>() / 360.0f);
    float xmax = ymax * aspectRatio;
    this->horizontalVerticalClipping = glm::vec4(-xmax, xmax, -ymax, ymax);
}

scene::Camera::~Camera(void)
{
}

void scene::Camera::setAspectRatio(const float &val)
{
    setProjection(val, this->fieldOfView, this->nearClipping, this->farClipping);
}

void scene::Camera::setNearClipping(const float &val)
{
    setProjection(this->aspectRatio, this->fieldOfView, val, this->farClipping);
}

void scene::Camera::setFarClipping(const float &val)
{
    setProjection(this->aspectRatio, this->fieldOfView, this->nearClipping, val);
}

void scene::Camera::setFieldOfView(const float &val)
{
    setProjection(this->aspectRatio, val, this->nearClipping, this->farClipping);
}
