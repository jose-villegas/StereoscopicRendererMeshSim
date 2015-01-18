#include "StoredShaders.h"
#include "..\Types\Texture.h"
using namespace collections::stored;

void collections::stored::StoredShaders::LoadShaders()
{
    shaders.resize(core::StoredShaders::Shaders::Count);
    // shared_data.glsl raw string to be added to include token
    std::string shared_data = types::Shader::fileToString(core::ShadersData::Filename());

    for (int i = 0; i < core::StoredShaders::Count; i++) {
        // reserve for new shader program
        types::ShaderProgram *shp = new types::ShaderProgram();
        // reserve new shaders
        types::Shader *vert = new types::Shader(types::Shader::Vertex);
        types::Shader *frag = new types::Shader(types::Shader::Fragment);
        // load shaders file to a string and concat shared_data.glsl
        vert->loadFromFile(core::StoredShaders::Filename((core::StoredShaders::Shaders)i, types::Shader::Vertex), "--include shared_data.glsl", shared_data);
        frag->loadFromFile(core::StoredShaders::Filename((core::StoredShaders::Shaders)i, types::Shader::Fragment), "--include shared_data.glsl", shared_data);
        // compile and verify fragment and vertex shaders
        vert->compile(); frag->compile();
        // attach to shader program after successful
        // compilation, link shaders with shader program
        shp->attachShader(vert); shp->attachShader(frag); shp->link();
        // add uniform and uniformblock data to shaderprogram
        AddShaderData(shp);

        // try to associate shaderprogram mapping textures
        for (int j = 1; j < types::Texture::Count; j++) {
            shp->addUniform(core::ShadersData::Samplers::NAMES[j]);
        }

        // add shaderprogram to class storage vector
        shaders[i] = shp;
    }
}

types::ShaderProgram *collections::stored::StoredShaders::getStoredShader(const core::StoredShaders::Shaders &sh)
{
    if (shaders.empty()) { return nullptr; }

    return shaders[sh];
}

void collections::stored::StoredShaders::Clear()
{
    for (auto it = shaders.begin(); it != shaders.end(); ++it) {
        delete *it;
    }

    shaders.clear();
}

void collections::stored::StoredShaders::AddShaderData(types::ShaderProgram *shp)
{
    // Elemental matrices uniform block
    shp->addUniformBlock(core::ShadersData::UniformBlocks::SHAREDMATRICES_NAME, 0);
    // Lights uniform block
    shp->addUniformBlock(core::ShadersData::UniformBlocks::SHAREDLIGHTS_NAME, 1);

    // Material Params
    for (int i = 0; i < core::ShadersData::Structures::MATERIAL_MEMBER_COUNT; i++) {
        shp->addUniform(std::string(core::ShadersData::Uniforms::MATERIAL_INSTANCE_NAME) + "." + std::string(core::ShadersData::Structures::MATERIAL_MEMBER_NAMES[i]));
    }
}

std::vector<types::ShaderProgram *> collections::stored::StoredShaders::shaders;
