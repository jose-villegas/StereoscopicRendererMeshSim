#include "Shaders.h"
using namespace collections::stored;

void collections::stored::Shaders::loadShaders()
{
    _shaders.resize(core::AvailableShaders::Shaders::Count);
    // Creating diffuse shader program
    types::ShaderProgram *shp = new types::ShaderProgram();
    types::Shader *vertex = new types::Shader(types::Shader::Vertex);
    types::Shader *fragment = new types::Shader(types::Shader::Fragment);
    std::string shared_data = types::Shader::fileToString("../TGC_SceneRenderer/resources/shaders/shared_data.glsl");
    vertex->loadFromFile("../TGC_SceneRenderer/resources/shaders/diffuse.vert", "--include shared_data.glsl", shared_data);
    fragment->loadFromFile("../TGC_SceneRenderer/resources/shaders/diffuse.frag", "--include shared_data.glsl", shared_data);
    vertex->compile();
    fragment->compile();
    shp->attachShader(vertex);
    shp->attachShader(fragment);
    shp->link();
    // Control Vars
    core::ShadersData::AddShaderData(shp);
    // Texture Maps
    shp->addUniform("diffuseMap");
    _shaders[core::AvailableShaders::Shaders::Diffuse] = shp;
}

types::ShaderProgram *collections::stored::Shaders::getDefaultShader(const core::AvailableShaders::Shaders &sh)
{
    if (_shaders.empty()) { return nullptr; }

    return _shaders[sh];
}

std::vector<types::ShaderProgram *> collections::stored::Shaders::_shaders;
