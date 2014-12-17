#include "Shaders.h"
using namespace collections::stored;

void collections::stored::Shaders::loadShaders()
{
    _shaders.resize(DefaultShaders::Count);
    // Creating diffuse shader program
    types::ShaderProgram *diffuseShader = new types::ShaderProgram();
    types::Shader *vertex = new types::Shader(types::Shader::Vertex, "../TGC_SceneRenderer/resources/shaders/diffuse.vert");
    types::Shader *fragment = new types::Shader(types::Shader::Fragment, "../TGC_SceneRenderer/resources/shaders/diffuse.frag");
    vertex->compile();
    fragment->compile();
    diffuseShader->attachShader(vertex);
    diffuseShader->attachShader(fragment);
    diffuseShader->link();
    // Elemental Matrices Params
    diffuseShader->addUniform("inputMatrices.modelViewProjection");
    diffuseShader->addUniform("inputMatrices.modelView");
    diffuseShader->addUniform("inputMatrices.model");
    diffuseShader->addUniform("inputMatrices.view");
    diffuseShader->addUniform("inputMatrices.projection");
    // Lights Params
    diffuseShader->addUniform("light.position");
    diffuseShader->addUniform("light.color");
    diffuseShader->addUniform("light.intensity");
    diffuseShader->addUniform("light.attenuation");
    diffuseShader->addUniform("light.direction");
    diffuseShader->addUniform("light.innerConeAngle");
    diffuseShader->addUniform("light.outerConeAngle");
    diffuseShader->addUniform("light.lightType");
    // Texture Maps
    diffuseShader->addUniform("diffuseMap");
    _shaders[Diffuse] = diffuseShader;
}

types::ShaderProgram *collections::stored::Shaders::getDefaultShader(const DefaultShaders &value)
{
    if (_shaders.empty()) { return nullptr; }

    return _shaders[value];
}

std::vector<types::ShaderProgram *> collections::stored::Shaders::_shaders;
