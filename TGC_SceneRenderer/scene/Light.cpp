#include "Light.h"
using namespace scene;

Light::Light(void)
{
    this->color          = glm::vec3(1.0f, 1.0f, 1.0f);
    this->attenuation    = 0.5f;
    this->intensity      = 1.0f;
    this->innerConeAngle = glm::radians(30.0f);
    this->outerConeAngle = glm::radians(90.0f);
    this->cosInnerConeAngle = std::cos(this->innerConeAngle);
    this->cosOuterConeAngle = std::cos(this->outerConeAngle);
    this->lightType      = Point;
    this->base = new bases::BaseObject("Light");
}

void scene::Light::setColor(const float &value0, const float &value1, const float &value2)
{
    this->color = glm::vec3(value0, value1, value2);
}

void scene::Light::setColor(const unsigned int &value0, const unsigned int &value1, const unsigned int &value2)
{
    setColor((float)value0 / 255.0f, (float)value1 / 255.0f, (float)value2 / 255.0f);
}

std::string scene::Light::getLightTypeString(const LightType &lightType)
{
    switch (lightType) {
        case LightType::Directional:
            return "Directional";

        case LightType::Point:
            return "Point";

        case LightType::Spot:
            return "Spot";
    }

    return "";
}

scene::Light::~Light(void)
{
    collections::LightsCollection::Instance()->removeLight(this);
}

scene::Light::Light(const LightType &lghtType)
{
    this->color          = glm::vec3(1.0f, 1.0f, 1.0f);
    this->attenuation    = 0.5f;
    this->intensity      = 1.0f;
    this->innerConeAngle = glm::radians(30.0f);
    this->outerConeAngle = glm::radians(90.0f);
    this->cosInnerConeAngle = std::cos(this->innerConeAngle);
    this->cosOuterConeAngle = std::cos(this->outerConeAngle);
    this->lightType      = lightType;
    this->base = new bases::BaseObject(getLightTypeString(lghtType) + " Light");
}

glm::vec3 scene::Light::getDirection()
{
    return glm::mat3_cast(this->base->transform.rotation) * glm::vec3(0.0, -1.0, 0.0);
}
