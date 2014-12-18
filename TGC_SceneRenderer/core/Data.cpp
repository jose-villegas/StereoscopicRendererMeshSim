#include "Data.h"
using namespace core;

const char *core::ShadersData::Structures::LIGHT_MEMBER_NAMES[] = {
    "position",
    "direction",
    "color",
    "intensity",
    "attenuation",
    "innerConeAngle",
    "outerConeAngle",
    "lightType"
};

const char *core::ShadersData::Structures::MATERIAL_MEMBER_NAMES[] = {
    "ambient"
    "diffuse"
    "specular"
    "shininess"
};

const char *core::ShadersData::Structures::MATRIX_MEMBER_NAMES[] = {
    "modelViewProjection"
    "modelView"
    "model"
    "view"
    "projection"
    "normal"
};

void core::ShadersData::AttachShaderData(types::ShaderProgram *shp)
{
    // Control Vars
    shp->addUniform("lightsCount");
    // Elemental Matrices Params
    shp->addUniform("inputMatrices.modelViewProjection");
    shp->addUniform("inputMatrices.modelView");
    shp->addUniform("inputMatrices.model");
    shp->addUniform("inputMatrices.view");
    shp->addUniform("inputMatrices.projection");
    shp->addUniform("inputMatrices.normal");
    // Material Params
    shp->addUniform("material.ambient");
    shp->addUniform("material.diffuse");
    shp->addUniform("material.specular");
    shp->addUniform("material.shininess");

    // Lights Params
    for (int i = 0; i < Constrains::MAX_LIGHTS; i++) {
        shp->addUniform("light[" + std::to_string(i) + "].position");
        shp->addUniform("light[" + std::to_string(i) + "].direction");
        shp->addUniform("light[" + std::to_string(i) + "].color");
        shp->addUniform("light[" + std::to_string(i) + "].intensity");
        shp->addUniform("light[" + std::to_string(i) + "].attenuation");
        shp->addUniform("light[" + std::to_string(i) + "].innerConeAngle");
        shp->addUniform("light[" + std::to_string(i) + "].outerConeAngle");
        shp->addUniform("light[" + std::to_string(i) + "].lightType");
    }
}
