#include "Frustum.h"
#include "glm\gtc\constants.hpp"
#include "glm\detail\func_trigonometric.hpp"
#include "glm\detail\func_geometric.inl"
using namespace types;

Frustum::Frustum(void)
{
}

void types::Frustum::setCameraProjectionParams(const float fov, const float aspectRatio, const float nearDistance, const float farDistance)
{
    // store camera information to frustum handlers
    this->aspectRatio = aspectRatio;
    this->fieldOfView = fov;
    this->nearDistance = nearDistance;
    this->farDistance = farDistance;
    // compute frustum planes width and height
    float tanG = (float)glm::tan(glm::radians(fov)  * 0.5f);
    // near width height
    nearHeight = nearDistance * tanG;
    nearWidth = nearHeight * aspectRatio;
    // far width height
    farHeight = farDistance * tanG;
    farWidth = farHeight * aspectRatio;
}

void types::Frustum::setCameraViewParams(const glm::vec3 &position, const glm::vec3 target, const glm::vec3 up)
{
    glm::vec3 direction, nearCenter, farCenter, cameraXAxis, cameraYAxis, cameraZAxis;
    // the camera z axis points to the oposite of the view direction
    cameraZAxis = glm::normalize(position - target);
    // camera x axis given the up vector, right vector perpendicular to the up vector
    cameraXAxis = glm::normalize(glm::cross(up, cameraZAxis));
    // frustum real up vector from z axis and x axis
    cameraYAxis = glm::cross(cameraZAxis, cameraYAxis);
    // compute near plane and far plane center points
    nearCenter = position - cameraZAxis * nearDistance;
    farCenter = position - cameraZAxis * farDistance;
    // compute vertex positions for the near plane
    nearTopLeft     = nearCenter + cameraYAxis * nearHeight - cameraXAxis * nearWidth;
    nearTopRight    = nearCenter + cameraYAxis * nearHeight + cameraXAxis * nearWidth;
    nearBottomLeft  = nearCenter - cameraYAxis * nearHeight - cameraXAxis * nearWidth;
    nearBottomRight = nearCenter - cameraYAxis * nearHeight + cameraXAxis * nearWidth;
    // compute vertex position for the far plane
    farTopLeft     = farCenter + cameraYAxis * farHeight - cameraXAxis * farWidth;
    farTopRight    = farCenter + cameraYAxis * farHeight + cameraXAxis * farWidth;
    farBottomLeft  = farCenter - cameraYAxis * farHeight - cameraXAxis * farWidth;
    farBottomRight = farCenter - cameraYAxis * farHeight + cameraXAxis * farWidth;
    // compute frustum far, near, top, bottom, left, right planes
    glm::vec3 aux, normal;
    cameraPlanes[Near].setPlane(-cameraZAxis, nearCenter);
    cameraPlanes[Far].setPlane(cameraZAxis, farCenter);
    // frustum top plane
    aux = glm::normalize((nearCenter + cameraYAxis * nearHeight) - position);
    normal = aux * cameraXAxis;
    cameraPlanes[Top].setPlane(normal, nearCenter + cameraYAxis * nearHeight);
    // frustum bottom plane
    aux = glm::normalize((nearCenter - cameraYAxis * nearHeight) - position);
    normal = cameraXAxis * aux;
    cameraPlanes[Bottom].setPlane(normal, nearCenter - cameraYAxis * nearHeight);
    // frustum left plane
    aux = glm::normalize((nearCenter - cameraXAxis * nearWidth) - position);
    normal = aux * cameraYAxis;
    cameraPlanes[Left].setPlane(normal, nearCenter - cameraXAxis * nearWidth);
    // frustum right plane
    aux = glm::normalize((nearCenter + cameraXAxis * nearWidth) - position);
    normal = cameraYAxis * aux;
    cameraPlanes[Right].setPlane(normal, nearCenter + cameraXAxis * nearWidth);
}
