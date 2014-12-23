#include "Transform.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

using namespace types;

Transform::Transform(void)
{
    this->position = glm::vec3(0.0, 0.0, 0.0);
    this->rotation = glm::quat(glm::vec3(0.0, 0.0, 0.0));
    this->scale = glm::vec3(1.0, 1.0, 1.0);
}

glm::mat4 types::Transform::getModelMatrix()
{
    return glm::mat4(glm::translate(this->position) * glm::mat4_cast(this->rotation) * glm::scale(this->scale));
}

void types::Transform::setPosition(const float &value0, const float &value1, const float &value2)
{
    this->position.x = value0;
    this->position.y = value1;
    this->position.z = value2;
}

void types::Transform::setRotation(const float &value0, const float &value1, const float &value2)
{
    this->rotation = glm::quat(glm::vec3(value0, value1, value2));
}

void types::Transform::setScale(const float &value0, const float &value1, const float &value2)
{
    this->scale.x = value0;
    this->scale.y = value1;
    this->scale.z = value2;
}

glm::vec3 types::Transform::eulerAngles()
{
    return glm::eulerAngles(this->rotation);
}

glm::quat types::Transform::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
    start = glm::normalize(start);
    dest = glm::normalize(dest);
    float cosTheta = glm::dot(start, dest);
    glm::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f) {
        // special case when vectors in opposite directions :
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        // This implementation favors a rotation around the Up axis,
        // since it's often what you want to do.
        rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);

        if (glm::length2(rotationAxis) < 0.01) { // bad luck, they were parallel, try again!
            rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
        }

        rotationAxis = glm::normalize(rotationAxis);
        return glm::angleAxis(180.0f, rotationAxis);
    }

    // Implementation from Stan Melax's Game Programming Gems 1 article
    rotationAxis = glm::cross(start, dest);
    float s = glm::sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;
    return glm::quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
}

glm::quat types::Transform::LookAt(glm::vec3 direction, glm::vec3 desiredUp)
{
    if (glm::length2(direction) < 0.0001f) {
        return glm::quat();
    }

    // Recompute desiredUp so that it's perpendicular to the direction
    // You can skip that part if you really want to force desiredUp
    glm::vec3 right = glm::cross(direction, desiredUp);
    desiredUp = glm::cross(right, direction);
    // Find the rotation between the front of the object (that we assume towards +Z,
    // but this depends on your model) and the desired direction
    glm::quat rot1 = RotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), direction);
    // Because of the 1rst rotation, the up is probably completely screwed up.
    // Find the rotation between the "up" of the rotated object, and the desired up
    glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
    glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);
    // Apply them
    return rot2 * rot1; // remember, in reverse order.
}

glm::quat types::Transform::RotateTowards(glm::quat q1, glm::quat q2, float maxAngle)
{
    if (maxAngle < 0.001f) {
        // No rotation allowed. Prevent dividing by 0 later.
        return q1;
    }

    float cosTheta = glm::dot(q1, q2);

    // q1 and q2 are already equal.
    // Force q2 just to be sure
    if (cosTheta > 0.9999f) {
        return q2;
    }

    // Avoid taking the long path around the sphere
    if (cosTheta < 0) {
        q1 = q1 * -1.0f;
        cosTheta *= -1.0f;
    }

    float angle = glm::acos(cosTheta);

    // If there is only a 2° difference, and we are allowed 5°,
    // then we arrived.
    if (angle < maxAngle) {
        return q2;
    }

    // This is just like slerp(), but with a custom t
    float t = maxAngle / angle;
    angle = maxAngle;
    glm::quat res = (glm::sin((1.0f - t) * angle) * q1 + glm::sin(t * angle) * q2) / glm::sin(angle);
    res = glm::normalize(res);
    return res;
}
