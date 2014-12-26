#include "Material.h"
#include "..\Collections\TexturesCollection.h"

using namespace types;

Material::Material(void)
{
    ambient = diffuse = specular = glm::vec3(0.5f);
    this->shininess = 16.0f;
    this->matShader = nullptr;

    // resize ShaderLink data vector for this structure
    for (int i = 0; i < core::ShadersData::Structures::MATERIAL_MEMBER_COUNT; i++) {
        LinkData data(std::string(core::ShadersData::Uniforms::MATERIAL_INSTANCE_NAME) + "." + std::string(core::ShadersData::Structures::MATERIAL_MEMBER_NAMES[i]), -1, i);
        uniformData.push_back(data);
    }
}

void types::Material::addTexture(Texture *tex)
{
    // if texture is null, cancel
    if (!tex) { return; }

    this->textures.insert(tex);
}

void types::Material::addTexture(Texture *tex[], const unsigned int texCount)
{
    if (texCount == 0) { return; }

    this->textures.insert(tex, tex + texCount);
}

void types::Material::setShaderProgram(ShaderProgram *shp)
{
    // Cancel if shp is null
    if (!shp) { return; }

    this->matShader = shp;
    // Save Locations based on the current shader
    // and stored names and indexes on uniformData
    saveUniformLocations(this->matShader);
}

void types::Material::setUniforms(types::ShaderProgram *shp)
{
    if (!shp) { return; }

    for (int i = 0; i < core::ShadersData::Structures::MATERIAL_MEMBER_COUNT; i++) {
        switch (i) {
            case 0:
                shp->setUniform(this->uniformData[i].uniformLocation, this->ambient);
                break;

            case 1:
                shp->setUniform(this->uniformData[i].uniformLocation, this->diffuse);
                break;

            case 2:
                shp->setUniform(this->uniformData[i].uniformLocation, this->specular);
                break;

            case 3:
                shp->setUniform(this->uniformData[i].uniformLocation, this->shininess);
                break;
        }
    }

    setTexturesUniforms();
}

void types::Material::setUniforms()
{
    setUniforms(this->matShader);
}

void types::Material::bindTextures() const
{
    // Bind textures in reverse type order
    for (auto it = this->textures.rbegin(); it != this->textures.rend(); it++) {
        ((Texture *)*it)->bind();
    }
}

void types::Material::guessMaterialShader()
{
    setShaderProgram(collections::stored::StoredShaders::getDefaultShader("Diffuse"));
}

void types::Material::setTexturesUniforms()
{
    setTexturesUniforms(this->matShader);
}

void types::Material::setTexturesUniforms(types::ShaderProgram *shp)
{
    for (auto it = this->textures.begin(); it != this->textures.end(); it++) {
        // Obtain texID and textype to query data info
        unsigned int texID = ((Texture *)*it)->geTexId();
        unsigned int texType = ((Texture *)*it)->getType();
        // Set to the texture map shader the current texture assigned ID
        shp->setUniform(core::ShadersData::Samplers2D::NAMES[texType], texID);
    }
}

void types::Material::useMaterialShader()
{
    this->matShader->use();
}

types::Material::~Material(void)
{
    for each(types::Texture * var in this->textures) {
        collections::TexturesCollection::Instance()->unloadTexture(var->geTexId());
    }
}
