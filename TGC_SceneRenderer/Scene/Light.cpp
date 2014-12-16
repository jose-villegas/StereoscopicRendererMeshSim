#include "Light.h"
using namespace Scene;

Light::Light(void)
{
    this->diffuse = this->specular = glm::vec3(1.0, 1.0, 1.0);
    this->attenuation = glm::vec3(1.0, 0.0, 0.0);
    this->intensity = 1.0f;
    this->innerConeAngle = 180;
    this->outerConeAngle = 180;
    this->lightType = Point;
}
