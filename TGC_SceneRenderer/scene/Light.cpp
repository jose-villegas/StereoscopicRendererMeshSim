#include "Light.h"
using namespace scene;

Light::Light(void)
{
    this->color          = glm::vec3(1.0, 1.0, 1.0);
    this->attenuation    = 1.0;
    this->intensity      = 1.0f;
    this->innerConeAngle = 30;
    this->outerConeAngle = 90;
    this->lightType      = Point;
    this->base = new bases::BaseObject("Light");
}

void scene::Light::setColor(const float &value0, const float &value1, const float &value2)
{
    this->color.r = value0;
    this->color.g = value1;
    this->color.b = value2;
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
}
