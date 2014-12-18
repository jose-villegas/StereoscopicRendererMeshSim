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
    uniformsSet          = false;

    // Resize ShaderLink data vector for this structure
    for (int i = 0; i < core::ShadersData::Structures::LIGHT_MEMBER_COUNT; i++) {
        LinkData data(core::ShadersData::Structures::LIGHT_MEMBER_NAMES[i], -1, i);
        uniformData.push_back(data);
    }
}

void scene::Light::setUniforms(types::ShaderProgram *sh)
{
    if (!uniformsSet) { return; }

    for (unsigned int i = 0; i < uniformData.size(); i++) {
        switch (i) {
            case 0:
                sh->setUniform(uniformData[i].uniformLocation, this->transform->position);
                break;

            case 1:
                sh->setUniform(uniformData[i].uniformLocation, glm::normalize(glm::vec3(0.0, -1.0, 0.0) * glm::mat3_cast(this->transform->rotation)));
                break;

            case 2:
                sh->setUniform(uniformData[i].uniformLocation, this->color);
                break;

            case 3:
                sh->setUniform(uniformData[i].uniformLocation, this->intensity);
                break;

            case 4:
                sh->setUniform(uniformData[i].uniformLocation, this->attenuation);
                break;

            case 5:
                sh->setUniform(uniformData[i].uniformLocation, this->innerConeAngle);
                break;

            case 6:
                sh->setUniform(uniformData[i].uniformLocation, this->outerConeAngle);
                break;

            case 7:
                sh->setUniform(uniformData[i].uniformLocation, this->lightType);
                break;

            default:
                break;
        }
    }
}
