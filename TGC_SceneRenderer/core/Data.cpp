#include "Data.h"
#include <string>
#include "..\Utils\Time.h"
#include "..\Utils\FrameRate.h"
#include "..\Collections\TexturesCollection.h"
#include "..\Collections\stored\StoredShaders.h"
#include "..\collections\LightsCollection.h"
using namespace core;

const char *core::ShadersData::UniformBlocks::SHAREDLIGHTS_INSTANCE_NAME = "light";

const char *core::ShadersData::UniformBlocks::SHAREDLIGHTS_NAME = "sharedLights";

const char *core::ShadersData::UniformBlocks::SHAREDMATRICES_INSTANCE_NAME = "matrix";

const char *core::ShadersData::UniformBlocks::SHAREDMATRICES_NAME = "sharedMatrices";

const char *core::ShadersData::Uniforms::MATERIAL_INSTANCE_NAME = "material";

GLchar *core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES[SHAREDLIGHTS_COMPLETE_COUNT];

const char *core::ShadersData::UniformBlocks::SHAREDLIGHTS_MEMBER_NAMES[] = {
    "source",
    "count"
};

const GLchar *core::ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_NAMES[] = {
    "sharedMatrices.modelViewProjection",
    "sharedMatrices.modelView",
    "sharedMatrices.model",
    "sharedMatrices.view",
    "sharedMatrices.projection",
    "sharedMatrices.normal"
};

const GLchar *core::ShadersData::Structures::LIGHT_MEMBER_NAMES[] = {
    "position",
    "direction",
    "color",
    "intensity",
    "attenuation",
    "innerConeAngle",
    "outerConeAngle",
    "lightType"
};

const GLchar *core::ShadersData::Structures::MATERIAL_MEMBER_NAMES[] = {
    "ambient",
    "diffuse",
    "specular",
    "shininess"
};

const char *core::ShadersData::Samplers2D::NAMES[] = {
    "noneMap",
    "diffuseMap",
    "specularMap",
    "ambientMap",
    "emissiveMap",
    "heightMap",
    "normalsMap",
    "shininessMap",
    "opacityMap",
    "displacementMap",
    "lightmapMap",
    "reflectionMap"
};

const char *core::StoredShaders::SHADER_NAMES[] = {
    "Diffuse"
};

const char *core::StoredMeshes::MESH_NAMES[] = {
    "cube",
    "cylinder",
    "sphere",
    "torus"
};

const char *core::StoredMeshes::MESH_FILENAMES[] = {
    "/resources/models/cube/cube.obj",
    "/resources/models/cylinder/cylinder.obj",
    "/resources/models/sphere/sphere.obj",
    "/resources/models/torus/torus.obj"
};


void core::ShadersData::AddShaderData(types::ShaderProgram *shp)
{
    // Elemental matrices uniform block
    shp->addUniformBlock(core::ShadersData::UniformBlocks::SHAREDMATRICES_NAME, 0);
    // Lights uniform block
    shp->addUniformBlock(core::ShadersData::UniformBlocks::SHAREDLIGHTS_NAME, 1);

    // Material Params
    for (int i = 0; i < Structures::MATERIAL_MEMBER_COUNT; i++) {
        shp->addUniform(std::string(Uniforms::MATERIAL_INSTANCE_NAME) + "." + std::string(Structures::MATERIAL_MEMBER_NAMES[i]));
    }
}

void core::ShadersData::CREATE_SHAREDLIGHTS_COMPLETE_NAMES(char *outNames[])
{
    for (int j = 0; j < EngineData::Constrains::MAX_LIGHTS; j++) {
        for (int k = 0; k < Structures::LIGHT_MEMBER_COUNT; k++) {
            // Current position
            int index = k + j * Structures::LIGHT_MEMBER_COUNT;
            // Result string
            std::string current = std::string(UniformBlocks::SHAREDLIGHTS_NAME) + "." + std::string(UniformBlocks::SHAREDLIGHTS_MEMBER_NAMES[0]);
            current += "[" + std::to_string(j) + "]." + std::string(Structures::LIGHT_MEMBER_NAMES[k]);
            outNames[index] = (char *)malloc(current.size() * sizeof(char));
            strcpy(outNames[index], current.c_str());
        }
    }

    std::string current = std::string(UniformBlocks::SHAREDLIGHTS_NAME) + "." + std::string(UniformBlocks::SHAREDLIGHTS_MEMBER_NAMES[1]);
    outNames[UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT - 1] = (char *)malloc(current.size() * sizeof(char));
    strcpy(outNames[UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT - 1], current.c_str());
}

void core::ShadersData::Initialize()
{
    // Only on call to this function
    if (dataSet) { return; }

    // Load Execution Location Info
    const std::string &execDirRef = core::ExecutionInfo::EXEC_DIR;
    DWORD cwdsz = GetCurrentDirectory(0, 0); // determine size needed
    char *cwd = (char *)malloc(cwdsz);

    if (GetCurrentDirectory(cwdsz, cwd) == 0) {
        return;
    } else {
        std::string result = std::string(cwd);
        std::replace(result.begin(), result.end(), '\\', '/');
        const_cast<std::string &>(execDirRef) = result;
    }

    free((void *)cwd);
    // Set up shared light huge string array of names
    CREATE_SHAREDLIGHTS_COMPLETE_NAMES(UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES);
    // Instace Singleton Engine Classes
    utils::Time::Instance();
    utils::FrameRate::Instance();
    collections::TexturesCollection::Instance();
    collections::LightsCollection::Instance();
    // Load Default Texture
    collections::TexturesCollection::Instance()->addTexture("../TGC_SceneRenderer/resources/default.png", 0, types::Texture::Diffuse);
    // Load Engine Shaders
    collections::stored::StoredShaders::LoadShaders();
    // Tells the class that the relevant data has been already construed and loaded
    dataSet = true;
}

bool core::ShadersData::dataSet = false;

const std::string core::ExecutionInfo::EXEC_DIR = "";
