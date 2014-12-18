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

    this->_textures.push_back(tex);
}

void types::Material::setShaderProgram(ShaderProgram *shp)
{
    // Cancel if shp is null
    if (!shp) { return; }

    this->_matShader = shp;
}
