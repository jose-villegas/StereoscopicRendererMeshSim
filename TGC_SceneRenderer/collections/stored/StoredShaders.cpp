#include "StoredShaders.h"
using namespace collections::stored;

void collections::stored::StoredShaders::LoadShaders()
{
    shaders.resize(core::StoredShaders::Shaders::Count);
    // Creating diffuse shader program
    types::ShaderProgram *shp = new types::ShaderProgram();
    types::Shader *vert = new types::Shader(types::Shader::Vertex);
    types::Shader *frag = new types::Shader(types::Shader::Fragment);
    std::string shared_data = types::Shader::fileToString(core::ShadersData::Filename());
    vert->loadFromFile(core::StoredShaders::Filename(core::StoredShaders::Diffuse, types::Shader::Vertex), "--include shared_data.glsl", shared_data);
    frag->loadFromFile(core::StoredShaders::Filename(core::StoredShaders::Diffuse, types::Shader::Fragment), "--include shared_data.glsl", shared_data);
    vert->compile();
    frag->compile();
    shp->attachShader(vert);
    shp->attachShader(frag);
    shp->link();
    // Control Vars
    core::ShadersData::AddShaderData(shp);
    // Texture Maps
    shp->addUniform("diffuseMap");
    shaders[core::StoredShaders::Shaders::Diffuse] = shp;
}

types::ShaderProgram *collections::stored::StoredShaders::getDefaultShader(const core::StoredShaders::Shaders &sh)
{
    if (shaders.empty()) { return nullptr; }

    return shaders[sh];
}

types::ShaderProgram *collections::stored::StoredShaders::getDefaultShader(const std::string &shaderName)
{
    for (int i = 0; i < core::StoredShaders::Count; i++) {
        if (core::StoredShaders::NAMES[i] == shaderName) {
            return shaders[i];
        }
    }

    return nullptr;
}

void collections::stored::StoredShaders::Clear()
{
    for (auto it = shaders.begin(); it != shaders.end(); ++it) {
        delete *it;
    }
}

std::vector<types::ShaderProgram *> collections::stored::StoredShaders::shaders;
