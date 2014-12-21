#include "Data.h"
#include <string>
#include "..\Utils\Time.h"
#include "..\Utils\FrameRate.h"
#include "..\Collections\TexturesCollection.h"
#include "..\Collections\stored\Shaders.h"
#include "..\collections\LightsCollection.h"
using namespace core;

const char *core::Data::UniformBlocks::SHAREDLIGHTS_INSTANCE_NAME = "light";

const char *core::Data::UniformBlocks::SHAREDLIGHTS_NAME = "sharedLights";

const char *core::Data::UniformBlocks::SHAREDMATRICES_INSTANCE_NAME = "matrix";

const char *core::Data::UniformBlocks::SHAREDMATRICES_NAME = "sharedMatrices";

const char *core::Data::Uniforms::MATERIAL_INSTANCE_NAME = "material";

GLchar *core::Data::UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES[SHAREDLIGHTS_COMPLETE_COUNT];

const char *core::Data::UniformBlocks::SHAREDLIGHTS_MEMBER_NAMES[] = {
    "source",
    "count"
};

const GLchar *core::Data::UniformBlocks::SHAREDMATRICES_MEMBER_NAMES[] = {
    "sharedMatrices.modelViewProjection",
    "sharedMatrices.modelView",
    "sharedMatrices.model",
    "sharedMatrices.view",
    "sharedMatrices.projection",
    "sharedMatrices.normal"
};

const GLchar *core::Data::Structures::LIGHT_MEMBER_NAMES[] = {
    "position",
    "direction",
    "color",
    "intensity",
    "attenuation",
    "innerConeAngle",
    "outerConeAngle",
    "lightType"
};

const GLchar *core::Data::Structures::MATERIAL_MEMBER_NAMES[] = {
    "ambient",
    "diffuse",
    "specular",
    "shininess"
};

const char *core::Data::Samplers2D::NAMES[] = {
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

const char *core::AvailableShaders::SHADER_NAMES[] = {
    "Diffuse"
};

void core::Data::AddShaderData(types::ShaderProgram *shp)
{
    // Elemental matrices uniform block
    shp->addUniformBlock(core::Data::UniformBlocks::SHAREDMATRICES_NAME, 0);
    // Lights uniform block
    shp->addUniformBlock(core::Data::UniformBlocks::SHAREDLIGHTS_NAME, 1);

    // Material Params
    for (int i = 0; i < Structures::MATERIAL_MEMBER_COUNT; i++) {
        shp->addUniform(std::string(Uniforms::MATERIAL_INSTANCE_NAME) + "." + std::string(Structures::MATERIAL_MEMBER_NAMES[i]));
    }
}

void core::Data::CREATE_SHAREDLIGHTS_COMPLETE_NAMES(char *outNames[])
{
    for (int j = 0; j < Constrains::MAX_LIGHTS; j++) {
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

void core::Data::Initialize()
{
    // Only on call to this function
    if (dataSet) { return; }

    // Set up shared light huge string array of names
    CREATE_SHAREDLIGHTS_COMPLETE_NAMES(UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES);
    // Instace Singleton Engine Classes
    utils::Time::Instance();
    utils::FrameRate::Instance();
    collections::TexturesCollection::Instance();
    collections::LightsCollection::Instance();
    // Load Default Texture
    collections::TexturesCollection::Instance()->loadTexture("../TGC_SceneRenderer/resources/default.png", 0, types::Texture::Diffuse);
    // Load Engine Shaders
    collections::stored::Shaders::LoadShaders();
    // Tells the class that the relevant data has been already construed and loaded
    dataSet = true;
}

bool core::Data::dataSet = false;
