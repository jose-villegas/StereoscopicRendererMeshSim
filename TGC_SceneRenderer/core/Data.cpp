#include "Data.h"
#include <string>
using namespace core;

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
    "ambient"
    "diffuse"
    "specular"
    "shininess"
};

const GLchar *core::ShadersData::Structures::MATRIX_MEMBER_NAMES[] = {
    "sharedMatrices.modelViewProjection",
    "sharedMatrices.modelView",
    "sharedMatrices.model",
    "sharedMatrices.view",
    "sharedMatrices.projection",
    "sharedMatrices.normal"
};

const char *core::ShadersData::Samplers2D::NAMES[] = {
    "noneMap"
    "diffuseMap"
    "specularMap"
    "ambientMap"
    "emissiveMap"
    "heightMap"
    "normalsMap"
    "shininessMap"
    "opacityMap"
    "displacementMap"
    "lightmapMap"
    "reflectionMap"
};

const char *core::ShadersData::Structures::UNIFORM_BLOCK_MATRICES_NAME = "sharedMatrices";
const char *core::ShadersData::Structures::UNIFORM_BLOCK_MATRICES_INSTANCE = "matrix";

GLchar **CONSTRUCT_LIGHTSOURCE_MEMBER_NAMES()
{
    std::ofstream out("out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    std::string value = std::string(ShadersData::Structures::UNIFORM_BLOCK_MATRICES_NAME);
    GLchar *result[ShadersData::Constrains::MAX_LIGHTS * ShadersData::Structures::LIGHT_MEMBER_COUNT];

    for (int i = 0; i < ShadersData::Constrains::MAX_LIGHTS; i++) {
        for (int j = 0; j < ShadersData::Structures::LIGHT_MEMBER_COUNT; j++) {
            int index = j + i * ShadersData::Constrains::MAX_LIGHTS;
            // Build Uniform Block String
            std::string cString = value + ".lightSource[" + std::to_string(i) + "]." + std::string(ShadersData::Structures::LIGHT_MEMBER_NAMES[j]);
            // Reserve the appropiate amount of memory
            result[index] = (GLchar *)malloc(cString.size() * sizeof(GLchar));
            // Copy the string to the array
            strcpy((char *)result[index], cString.c_str());
            std::cout << cString << std::endl;
        }
    }

    std::cout.rdbuf(coutbuf); //reset to standard output again
    return result;
}

GLchar **core::ShadersData::Structures::LIGHTSOURCE_MEMBER_NAMES = CONSTRUCT_LIGHTSOURCE_MEMBER_NAMES();


void core::ShadersData::AddShaderData(types::ShaderProgram *shp)
{
    // Elemental matrices uniform block
    shp->addUniformBlock(core::ShadersData::Structures::UNIFORM_BLOCK_MATRICES_NAME);
    // Material Params
    shp->addUniform("material.ambient");
    shp->addUniform("material.diffuse");
    shp->addUniform("material.specular");
    shp->addUniform("material.shininess");

    // Lights Params
    for (int i = 0; i < Constrains::MAX_LIGHTS; i++) {
        //shp->addUniform("light[" + std::to_string(i) + "].position");
        //shp->addUniform("light[" + std::to_string(i) + "].direction");
        //shp->addUniform("light[" + std::to_string(i) + "].color");
        //shp->addUniform("light[" + std::to_string(i) + "].intensity");
        //shp->addUniform("light[" + std::to_string(i) + "].attenuation");
        //shp->addUniform("light[" + std::to_string(i) + "].innerConeAngle");
        //shp->addUniform("light[" + std::to_string(i) + "].outerConeAngle");
        //shp->addUniform("light[" + std::to_string(i) + "].lightType");
    }

    // Control Vars
    shp->addUniform("lightsCount");
}


