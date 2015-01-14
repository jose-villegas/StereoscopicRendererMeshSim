#include "..\Collections\SceneObjectsCollection.h"
#include "..\Collections\TexturesCollection.h"
#include "..\Collections\stored\StoredShaders.h"
#include "..\Types\Shader.h"
#include "..\Utils\FrameRate.h"
#include "..\Utils\Time.h"
#include "..\collections\CamerasCollection.h"
#include "..\collections\LightsCollection.h"
#include "..\collections\MeshesCollection.h"
#include "Data.h"
#include <string>
#include <thread>
using namespace core;

const char *core::ShadersData::UniformBlocks::SHAREDLIGHTS_INSTANCE_NAME = "light";

const char *core::ShadersData::UniformBlocks::SHAREDLIGHTS_NAME = "sharedLights";

const char *core::ShadersData::UniformBlocks::SHAREDMATRICES_INSTANCE_NAME = "matrix";

const char *core::ShadersData::UniformBlocks::SHAREDMATRICES_NAME = "sharedMatrices";

const char *core::ShadersData::Uniforms::MATERIAL_INSTANCE_NAME = "material";

const char *core::ShadersData::FILENAME = "/resources/shaders/shared_data.glsl";

const char *core::ShadersData::Samplers2D::DEFAULT_TEX_FILENAME = "/resources/default.png";

GLchar *core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES[SHAREDLIGHTS_COMPLETE_COUNT];

const std::string core::ShadersData::Filename()
{
    return ExecutionInfo::EXEC_DIR + FILENAME;
}

const char *core::ShadersData::UniformBlocks::SHAREDLIGHTS_MEMBER_NAMES[] = {
    "source",
    "ambientLight",
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
    "cosInnerConeAngle",
    "cosOuterConeAngle",
    "lightType",

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

const char *core::StoredShaders::NAMES[] = {
    "Diffuse",
    "Specular",
    "OpacityDiffuse",
    "OpacitySpecular",
    "BumpedDiffuse",
    "BumpedSpecular",
    "BumpedOpacityDiffuse",
    "BumpedOpacitySpecular"
};

const char *core::StoredShaders::FILENAMES[] = {
    "/resources/shaders/diffuse",
    "/resources/shaders/specular",
    "/resources/shaders/opacity_diffuse",
    "/resources/shaders/opacity_specular",
    "/resources/shaders/bumped_diffuse",
    "/resources/shaders/bumped_specular",
    "/resources/shaders/bumped_opacity_diffuse",
    "/resources/shaders/bumped_opacity_specular"
};

const std::string core::StoredShaders::Filename(const Shaders &index, const unsigned int &type)
{
    std::string extension = ".";

    switch (type) {
        case types::Shader::Vertex:
            extension += "vert";
            break;

        case types::Shader::Fragment:
            extension += "frag";
            break;

        case types::Shader::Geometry:
            extension += "geom";
            break;
    }

    return ExecutionInfo::EXEC_DIR + FILENAMES[index] + extension;
}

const std::string core::StoredMeshes::Filename(const unsigned int &index)
{
    return ExecutionInfo::EXEC_DIR + FILENAMES[index];
}

const char *core::StoredMeshes::NAMES[] = {
    "Cube",
    "Cylinder",
    "Sphere",
    "Torus"
};

const char *core::StoredMeshes::FILENAMES[] = {
    "/resources/models/cube/cube.obj",
    "/resources/models/cylinder/cylinder.obj",
    "/resources/models/sphere/sphere.obj",
    "/resources/models/torus/torus.obj"
};

void core::ShadersData::CREATE_SHAREDLIGHTS_COMPLETE_NAMES(char *outNames[])
{
    // source lights structures
    for (int j = 0; j < EngineData::Constrains::MAX_LIGHTS; j++) {
        for (int k = 0; k < Structures::LIGHT_MEMBER_COUNT; k++) {
            // Current position
            int index = k + j * Structures::LIGHT_MEMBER_COUNT;
            // Result string
            std::string current = std::string(UniformBlocks::SHAREDLIGHTS_NAME) + "." + std::string(UniformBlocks::SHAREDLIGHTS_MEMBER_NAMES[0]);
            current += "[" + std::to_string(j) + "]." + std::string(Structures::LIGHT_MEMBER_NAMES[k]);
            outNames[index] = new char[current.size() + 1]; // include null character
            strcpy_s(outNames[index], current.size() + 1, current.c_str());
        }
    }

    // rest single members
    for (int i = 1; i < UniformBlocks::SHAREDLIGHTS_MEMBER_COUNT; i++) {
        std::string current = std::string(UniformBlocks::SHAREDLIGHTS_NAME) + "." + std::string(UniformBlocks::SHAREDLIGHTS_MEMBER_NAMES[i]);
        outNames[UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT - UniformBlocks::SHAREDLIGHTS_MEMBER_COUNT + i] = new char[current.size() + 1];;
        strcpy_s(outNames[UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT - UniformBlocks::SHAREDLIGHTS_MEMBER_COUNT + i], current.size() + 1, current.c_str());
    }
}

void core::Data::Initialize()
{
    // Only on call to this function
    if (dataSet) { return; }

    int extensionNumber;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionNumber);

    for (int i = 0; i < extensionNumber; i++) {
        std::string currentExt = (char *)glGetStringi(GL_EXTENSIONS, i);

        if (currentExt == "GL_EXT_texture_filter_anisotropic") {
            EngineData::anisotropicFilteringAvailable = true;
            glGetFloatv(MAX_TEXTURE_MAX_ANISOTROPY_EXT, &EngineData::maxAnisotropicFiltering);
            break;
        }
    }

    // Load Execution Location Info - WIN only
    const std::string &execDirRef = core::ExecutionInfo::EXEC_DIR;
    // Obtain Execution Directory
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
    core::ShadersData::CREATE_SHAREDLIGHTS_COMPLETE_NAMES(core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES);
    // Instace Singleton Engine Classes
    utils::FrameRate::Instance();
    utils::Time::Instance();
    collections::LightsCollection::Instance();
    collections::MeshesCollection::Instance();
    collections::CamerasCollection::Instance();
    collections::SceneObjectsCollection::Instance();
    collections::TexturesCollection::Instance();
    // Load Default Texture
    collections::TexturesCollection::Instance()->addTexture(core::ExecutionInfo::EXEC_DIR + core::ShadersData::Samplers2D::DEFAULT_TEX_FILENAME, types::Texture::Diffuse);
    // Load Engine Shaders
    collections::stored::StoredShaders::LoadShaders();
    // Tells the class that the relevant data has been already construed and loaded
    dataSet = true;
}

void core::Data::Clear()
{
    // delete stored shaders and materials
    collections::stored::StoredShaders::Clear();
    // it is important to delete sceneobjects first as the topmost collection
    delete collections::SceneObjectsCollection::Instance();
    // delete scene components collections
    delete collections::CamerasCollection::Instance();
    delete collections::LightsCollection::Instance();
    delete collections::MeshesCollection::Instance();
    // delete texture collections
    delete collections::TexturesCollection::Instance();
    // delete memory reserved by utils
    delete utils::FrameRate::Instance();
    delete utils::Time::Instance();
}

bool core::Data::dataSet = false;;

const std::string core::ExecutionInfo::EXEC_DIR = "";

unsigned const int core::ExecutionInfo::AVAILABLE_CORES = std::thread::hardware_concurrency() <= 0 ? 1 : std::thread::hardware_concurrency();

bool core::EngineData::anisotropicFilteringAvailable = false;

GLfloat core::EngineData::maxAnisotropicFiltering = (GLfloat)0.0f;
