#include "Shaders.h"
using namespace collections::stored;

void collections::stored::Shaders::loadShaders()
{
    _shaders.resize(DefaultShaders::Count);
    // Creating diffuse shader program
    types::ShaderProgram *phongShading = new types::ShaderProgram();
    types::Shader *vertex = new types::Shader(types::Shader::Vertex);
    types::Shader *fragment = new types::Shader(types::Shader::Fragment);
    vertex->loadFromFile("../TGC_SceneRenderer/resources/shaders/diffuse.vert", "--include Constrains", core::Constrains::ToGLSL());
    fragment->loadFromFile("../TGC_SceneRenderer/resources/shaders/diffuse.frag", "--include Constrains", core::Constrains::ToGLSL());
    vertex->compile();
    fragment->compile();
    phongShading->attachShader(vertex);
    phongShading->attachShader(fragment);
    phongShading->link();
    // Elemental Matrices Params
    phongShading->addUniform("inputMatrices.modelViewProjection");
    phongShading->addUniform("inputMatrices.modelView");
    phongShading->addUniform("inputMatrices.model");
    phongShading->addUniform("inputMatrices.view");
    phongShading->addUniform("inputMatrices.projection");
    phongShading->addUniform("inputMatrices.normal");
    // Material Params
    phongShading->addUniform("material.ka");
    phongShading->addUniform("material.kd");
    phongShading->addUniform("material.ks");
    phongShading->addUniform("material.shininess");
    // Texture Maps
    phongShading->addUniform("diffuseMap");
    // Control Vars
    phongShading->addUniform("lightsCount");

    // Lights Params
    for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
        phongShading->addUniform("light[" + std::to_string(i) + "].position");
        phongShading->addUniform("light[" + std::to_string(i) + "].color");
        phongShading->addUniform("light[" + std::to_string(i) + "].intensity");
        phongShading->addUniform("light[" + std::to_string(i) + "].attenuation");
        phongShading->addUniform("light[" + std::to_string(i) + "].direction");
        phongShading->addUniform("light[" + std::to_string(i) + "].innerConeAngle");
        phongShading->addUniform("light[" + std::to_string(i) + "].outerConeAngle");
        phongShading->addUniform("light[" + std::to_string(i) + "].lightType");
    }

    _shaders[PhongShading] = phongShading;
}

types::ShaderProgram *collections::stored::Shaders::getDefaultShader(const DefaultShaders &value)
{
    if (_shaders.empty()) { return nullptr; }

    return _shaders[value];
}

std::vector<types::ShaderProgram *> collections::stored::Shaders::_shaders;
