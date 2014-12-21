#include "Light.h"
using namespace scene;

Light::Light(void)
{
    this->color          = glm::vec3(1.0, 1.0, 1.0);
    this->attenuation    = 1.0;
    this->intensity      = 1.0f;
    this->innerConeAngle = 180;
    this->outerConeAngle = 180;
    this->lightType      = Point;
}