#include "Material.h"

using namespace types;

Material::Material(void)
{
    ambient = diffuse = specular = glm::vec3(0.0f);
    this->shininess = 1.0f;

    // resize ShaderLink data vector for this structure
    for (int i = 0; i < core::ShadersData::Structures::MATERIAL_MEMBER_COUNT; i++) {
        LinkData data(core::ShadersData::Structures::MATERIAL_MEMBER_NAMES[i], -1, i);
        uniformData.push_back(data);
    }
}

void types::Material::addTexture(Texture *tex)
{
    // if texture is null, cancel
    if (!tex) { return; }

    this->_textures.insert(tex);
}

void types::Material::addTexture(Texture *tex[], const unsigned int texCount)
{
    if (texCount == 0) { return; }

    this->_textures.insert(tex, tex + texCount);
}

void types::Material::setShaderProgram(ShaderProgram *shp)
{
    // Cancel if shp is null
    if (!shp) { return; }

    this->_matShader = shp;
}

void types::Material::setUniforms(types::ShaderProgram *shp)
{
    for (int i = 0; i < core::ShadersData::Structures::MATERIAL_MEMBER_COUNT; i++) {
        switch (i) {
            case 0:
                shp->setUniform(uniformData[i].uniformLocation, this->ambient);
                break;

            case 1:
                shp->setUniform(uniformData[i].uniformLocation, this->diffuse);
                break;

            case 2:
                shp->setUniform(uniformData[i].uniformLocation, this->specular);
                break;

            case 3:
                shp->setUniform(uniformData[i].uniformLocation, this->shininess);
                break;
        }
    }
}
