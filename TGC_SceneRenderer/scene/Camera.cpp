#include "Camera.h"
#include "..\Core\Renderer.h"
#include "..\collections\MeshesCollection.h"
using namespace scene;

Camera::Camera(void)
{
    this->projectionType = Perspective;
    // normal members
    this->nearClippingPlane = 0.1f;
    this->farClippingPlane = 1000.f;
    this->vectorUp = glm::vec3(0.f, 1.f, 0.f);
    this->fieldOfView = 75.0f;
    this->aspectRatio = 16.0f / 9.0f;
    // ortho members
    this->orthoProjectionSize = 50.f;
    // stereo members
    this->zeroParallax = 1000.f;
    this->eyeSeparation = 0.133f;
    // subclass members
    this->base = new bases::BaseObject("Camera");
    setProjection(aspectRatio, fieldOfView, nearClippingPlane, farClippingPlane);
}

glm::mat4 scene::Camera::getViewMatrix(const glm::vec3 &cameraPosition, const glm::vec3 &cameraTarget, const glm::vec3 &vectorUp) const
{
    return glm::lookAt(cameraPosition, cameraTarget, vectorUp);
}

glm::mat4 scene::Camera::getViewMatrix()
{
    glm::vec3 cameraPosition = this->base->transform.position;
    glm::vec3 cameraTarget = this->getCameraTarget();
    glm::vec3 vecUp = this->calculateVectorUp();
    return glm::lookAt(cameraPosition, cameraTarget, vecUp);
}

glm::mat4 scene::Camera::getProjectionTypeMatrix() const
{
    if (this->projectionType == Perspective) {
        return getFrustumMatrix();
    } else if (this->projectionType == Orthographic) {
        return getOrthographicMatrix();
    }

    return  getFrustumMatrix();
}

glm::mat4 scene::Camera::getFrustumMatrix() const
{
    return glm::frustum(this->horizontalVerticalClipping.x,
                        this->horizontalVerticalClipping.y,
                        this->horizontalVerticalClipping.z,
                        this->horizontalVerticalClipping.w,
                        this->nearClippingPlane,
                        this->farClippingPlane);
}

glm::mat4 scene::Camera::getOrthographicMatrix() const
{
    return glm::ortho(this->horizontalVerticalClipping.x * this->orthoProjectionSize,
                      this->horizontalVerticalClipping.y * this->orthoProjectionSize,
                      this->horizontalVerticalClipping.z * this->orthoProjectionSize,
                      this->horizontalVerticalClipping.w * this->orthoProjectionSize,
                      this->nearClippingPlane,
                      this->farClippingPlane);
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
    this->nearClippingPlane = nearClipping;
    this->farClippingPlane = farClipping;
    // calculate vector up based on rotation transform
    this->calculateVectorUp();
    // Set ViewPort Accordly
    float ymax = nearClippingPlane * glm::tan(fieldOfView * glm::pi<float>() / 360.f); // (fov * pi / 180) / 2
    float xmax = ymax * aspectRatio;
    this->setViewPortRect(-xmax, xmax, -ymax, ymax);
}

scene::Camera::~Camera(void)
{
    collections::CamerasCollection::Instance()->removeCamera(this);
}

void scene::Camera::setAspectRatio(const float val)
{
    setProjection(val, this->fieldOfView, this->nearClippingPlane, this->farClippingPlane);
}

void scene::Camera::setNearClippingDistance(const float val)
{
    setProjection(this->aspectRatio, this->fieldOfView, val, this->farClippingPlane);
}

void scene::Camera::setFarClippingDistance(const float val)
{
    setProjection(this->aspectRatio, this->fieldOfView, this->nearClippingPlane, val);
}

void scene::Camera::setFieldOfView(const float val)
{
    setProjection(this->aspectRatio, val, this->nearClippingPlane, this->farClippingPlane);
}

void scene::Camera::setEyeSeparation(const float val)
{
    this->eyeSeparation = val;
}


void scene::Camera::renderFromPOV(const core::Renderer *actRenderer)
{
    // clear background color and buffers bits
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // obtain view matrix, it doesn't change through the whole loop
    glm::mat4 viewMatrix = this->getViewMatrix();

    // actrenderer elemental matrices for shader use
    if (this->projectionType == Stereoscopic) {
        // translated view matrices
        glm::mat4 leftViewMatrix = viewMatrix * glm::translate(glm::vec3(this->eyeSeparation / 2.f, 0.f, 0.f));
        glm::mat4 rightViewMatrix = viewMatrix * glm::translate(glm::vec3(-this->eyeSeparation / 2.f, 0.f, 0.f));
        // render from left pov
        actRenderer->matrices->setProjectionMatrix(this->leftFrustum());
        // displace world to the right
        actRenderer->matrices->setViewMatrix(leftViewMatrix);
        // update lights with new translated view matrix
        actRenderer->lights->setViewMatrix(leftViewMatrix);
        // sets the light uniform block with active lights params
        actRenderer->lights->setUniformBlock();
        // render in red
        glColorMask(true, false, false, false);
        // render all meshes from pov
        renderMeshes(actRenderer);
        // clear depth to avoid depth test
        glClear(GL_DEPTH_BUFFER_BIT) ;
        // render from right pov
        actRenderer->matrices->setProjectionMatrix(this->rightFrustum());
        // displace world to the left
        actRenderer->matrices->setViewMatrix(rightViewMatrix);
        // update lights with new translated view matrix
        actRenderer->lights->setViewMatrix(rightViewMatrix);
        // sets the light uniform block with active lights params
        actRenderer->lights->setUniformBlock();
        // render to cyan
        glColorMask(false, true, true, false);
        // render all meshes from pov
        renderMeshes(actRenderer);
        // restore original color mask
        glColorMask(true, true, true, true);
    } else {
        actRenderer->matrices->setViewMatrix(viewMatrix) ;
        actRenderer->matrices->setProjectionMatrix(this->getProjectionTypeMatrix());
        // update lights with new translated view matrix
        actRenderer->lights->setViewMatrix(viewMatrix);
        // sets the light uniform block with active lights params
        actRenderer->lights->setUniformBlock();
        // render all meshes from pov
        renderMeshes(actRenderer);
    }
}

glm::mat4 scene::Camera::leftFrustum()
{
    float top, bottom, left, right;
    // vertical planes stay the same
    top = this->horizontalVerticalClipping[3];
    bottom = this->horizontalVerticalClipping[2];
    // calculate horizontal planes movement
    float horizontalClipStep = this->aspectRatio * glm::tan(this->fieldOfView * glm::pi<float>() / 360.f) * this->zeroParallax;
    float leftSeparationStep = horizontalClipStep - this->eyeSeparation / 2.f;
    float rightSeparationStep = horizontalClipStep + this->eyeSeparation / 2.f;
    // horizontal planes movement
    left = -leftSeparationStep * this->nearClippingPlane / this->zeroParallax;
    right = rightSeparationStep * this->nearClippingPlane / this->zeroParallax;
    // return projection matrix
    return glm::frustum(left, right, bottom, top, this->nearClippingPlane, this->farClippingPlane);
}

glm::mat4 scene::Camera::rightFrustum()
{
    float top, bottom, left, right;
    // vertical planes stay the same
    top = this->horizontalVerticalClipping[3];
    bottom = this->horizontalVerticalClipping[2];
    // calculate horizontal planes movement
    float horizontalClipStep = this->aspectRatio * glm::tan(this->fieldOfView * glm::pi<float>() / 360.f) * this->zeroParallax;
    float leftSeparationStep = horizontalClipStep - this->eyeSeparation / 2.f;
    float rightSeparationStep = horizontalClipStep + this->eyeSeparation / 2.f;
    // horizontal planes movement
    left = -rightSeparationStep * this->nearClippingPlane / this->zeroParallax;
    right = leftSeparationStep * this->nearClippingPlane / this->zeroParallax;
    // return projection matrix
    return glm::frustum(left, right, bottom, top, this->nearClippingPlane, this->farClippingPlane);
}

void scene::Camera::renderMeshes(const core::Renderer *actRenderer)
{
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

glm::vec3 scene::Camera::getCameraTarget() const
{
    glm::vec3 cameraTarget = glm::mat3_cast(this->base->transform.rotation) * glm::vec3(0.0, 0.0, -1.0);
    return cameraTarget + this->base->transform.position;
}

const glm::vec3 &scene::Camera::calculateVectorUp()
{
    return this->vectorUp = glm::mat3_cast(this->base->transform.rotation) * glm::vec3(0.0, 1.0, 0.0);
}
