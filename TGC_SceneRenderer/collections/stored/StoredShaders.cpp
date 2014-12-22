#include "StoredShaders.h"
using namespace collections::stored;

void collections::stored::StoredShaders::LoadShaders()
{
    _shaders.resize(core::StoredShaders::Shaders::Count);
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
    _shaders[core::StoredShaders::Shaders::Diffuse] = shp;
}

types::ShaderProgram *collections::stored::StoredShaders::getDefaultShader(const core::StoredShaders::Shaders &sh)
{
    if (_shaders.empty()) { return nullptr; }

    return _shaders[sh];
}

types::ShaderProgram *collections::stored::StoredShaders::getDefaultShader(const std::string &shaderName)
{
    for (int i = 0; i < core::StoredShaders::Count; i++) {
        if (core::StoredShaders::SHADER_NAMES[i] == shaderName) {
            return _shaders[i];
        }
    }
}

std::vector<types::ShaderProgram *> collections::stored::StoredShaders::_shaders;
