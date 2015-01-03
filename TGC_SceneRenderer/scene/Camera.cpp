#include "Camera.h"
#include "..\Core\Renderer.h"
#include "..\collections\MeshesCollection.h"
using namespace scene;

Camera::Camera(void)
{
    this->projectionType = Perspective;
    this->nearClippingPlane.distance = 0.1f;
    this->farClippingPlane.distance = 1000.f;
    this->farClippingPlane.distance = 1000.0f;
    this->nearClippingPlane.distance = 0.1f;
    this->vectorUp = glm::vec3(0.f, 1.f, 0.f);
    this->fieldOfView = 75.0f;
    this->aspectRatio = 16.0f / 9.0f;
    this->base = new bases::BaseObject("Camera");
    setProjection(aspectRatio, fieldOfView, nearClippingPlane.distance, farClippingPlane.distance);
}

glm::mat4 scene::Camera::getViewMatrix(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &vectorUp) const
{
    return glm::lookAt(cameraPosition, cameraTarget, vectorUp);
}

glm::mat4 scene::Camera::getViewMatrix() const
{
    glm::vec3 cameraPosition = this->base->transform.position;
    glm::vec3 cameraTarget = (this->base->transform.position + glm::vec3(0.0, 0.0, -1.0)) * glm::mat3_cast(this->base->transform.rotation);
    return getViewMatrix(cameraPosition, cameraTarget, this->vectorUp);
}

glm::mat4 scene::Camera::getProjectionTypeMatrix() const
{
    if (this->projectionType == Perspective) {
        return glm::perspective(glm::radians(this->fieldOfView), this->aspectRatio, this->nearClippingPlane.distance, this->farClippingPlane.distance);
    } else if (this->projectionType == Orthographic) {
        return glm::ortho(this->horizontalVerticalClipping.x, this->horizontalVerticalClipping.y, this->horizontalVerticalClipping.z, this->horizontalVerticalClipping.w, this->nearClippingPlane.distance,
                          this->farClippingPlane.distance);
    }

    return glm::mat4(1.0);
}

glm::mat4 scene::Camera::getFrustumMatrix() const
{
    return glm::frustum(this->horizontalVerticalClipping.x, this->horizontalVerticalClipping.y, this->horizontalVerticalClipping.z, this->horizontalVerticalClipping.w, this->nearClippingPlane.distance,
                        this->farClippingPlane.distance);
}

glm::mat4 scene::Camera::getOrthographicMatrix() const
{
    return glm::ortho(this->horizontalVerticalClipping.x, this->horizontalVerticalClipping.y, this->horizontalVerticalClipping.z, this->horizontalVerticalClipping.w, this->nearClippingPlane.distance,
                      this->farClippingPlane.distance);
}

void scene::Camera::setViewPortRect(const float &left, const float &right, const float &bottom, const float &top)
{
    this->horizontalVerticalClipping = glm::vec4(left, right, bottom, top);
}

void scene::Camera::setProjection(const float &aspectRatio, const float &fieldOfView, const float &nearClipping, const float &farClipping)
{
    this->fieldOfView = fieldOfView;
    this->aspectRatio = aspectRatio;
    // update planes
    this->nearClippingPlane.distance = nearClipping;
    this->farClippingPlane.distance = farClipping;
    // planes bounds
    this->nearClippingPlane.height = 2.f * glm::tan(this->fieldOfView / 2.f) * this->nearClippingPlane.distance;
    this->nearClippingPlane.width = this->nearClippingPlane.height * this->aspectRatio;
    this->farClippingPlane.height = 2.f * glm::tan(this->fieldOfView / 2.f) * this->farClippingPlane.distance;
    this->farClippingPlane.width = this->farClippingPlane.height * this->aspectRatio;

    // planes points
    if (this->base) {
        glm::vec3 cameraPosition = this->base->transform.position;
        glm::vec3 cameraTarget = (this->base->transform.position + glm::vec3(0.0, 0.0, -1.0)) * glm::mat3_cast(this->base->transform.rotation);
        glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
        this->farClippingPlane.updatePoints(cameraTarget, cameraDirection, vectorUp, glm::cross(vectorUp, cameraDirection));
        this->nearClippingPlane.updatePoints(cameraTarget, cameraDirection, vectorUp, glm::cross(vectorUp, cameraDirection));
    }

    // Set ViewPort Accordly
    float ymax = nearClipping * glm::tan(fieldOfView * glm::pi<float>() / 360.f);
    float xmax = ymax * aspectRatio;
    this->setViewPortRect(-xmax, xmax, -ymax, ymax);
}

void scene::Camera::Plane::updatePoints(glm::vec3 position, glm::vec3 direction, glm::vec3 up, glm::vec3 right)
{
    glm::vec3 planePosition =  position + direction * this->distance;
    this->points[0] = planePosition - (up * height / 2.f) - (right * width / 2.f);
    this->points[1] = planePosition - (up * height / 2.f) + (right * width / 2.f);
    this->points[2] = planePosition + (up * height / 2.f) - (right * width / 2.f);
    this->points[3] = planePosition + (up * height / 2.f) + (right * width / 2.f);
}


scene::Camera::~Camera(void)
{
    collections::CamerasCollection::Instance()->removeCamera(this);
}

void scene::Camera::setAspectRatio(const float &val)
{
    setProjection(val, this->fieldOfView, this->nearClippingPlane.distance, this->farClippingPlane.distance);
}

void scene::Camera::setNearClippingDistance(const float &val)
{
    setProjection(this->aspectRatio, this->fieldOfView, val, this->farClippingPlane.distance);
}

void scene::Camera::setFarClippingDistance(const float &val)
{
    setProjection(this->aspectRatio, this->fieldOfView, this->nearClippingPlane.distance, val);
}

void scene::Camera::setFieldOfView(const float &val)
{
    setProjection(this->aspectRatio, val, this->nearClippingPlane.distance, this->farClippingPlane.distance);
}

void scene::Camera::renderFromPOV(const core::Renderer *actRenderer)
{
    // sets the light uniform block with active lights params
    actRenderer->lights->setUniformBlock();
    // actrenderer elemental matrices for shader use
    actRenderer->matrices->setViewMatrix(this->getViewMatrix());
    actRenderer->matrices->setProjectionMatrix(this->getFrustumMatrix());

    for (unsigned int i = 0; i < actRenderer->meshes->meshCount(); i++) {
        // set model view matrix per mesh
        actRenderer->matrices->setModelMatrix(actRenderer->meshes->getMesh(i)->base->transform.getModelMatrix());
        // recalculate matrices with current loaded matrices
        actRenderer->matrices->calculateMatrices();
        // update matrices uniform block data
        actRenderer->matrices->setUniformBlock();
        // finally call glDraw with mesh data
        actRenderer->meshes->getMesh(i)->render();
    }
}
