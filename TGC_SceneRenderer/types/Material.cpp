#include "Material.h"
#include "..\Collections\TexturesCollection.h"

using namespace types;

Material::Material(void)
{
    ambient = diffuse = specular = glm::vec3(0.5f);
    this->shininess = 16.0f;
    this->emission = glm::vec3(0.5);
    this->matShader = nullptr;
    this->hasTextureType = this->shaderTextures = std::vector<bool>(core::ShadersData::Samplers2D::Count);
    std::fill(this->hasTextureType.begin(), this->hasTextureType.end(), false);
    std::fill(this->shaderTextures.begin(), this->shaderTextures.end(), false);

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
    this->hasTextureType[(int)tex->getType()] = true;
}

void types::Material::addTexture(Texture *tex[], const unsigned int texCount)
{
    if (texCount == 0) { return; }

    this->textures.insert(tex, tex + texCount);

    for (unsigned int i = 0; i < texCount; i++) { this->hasTextureType[(int)tex[i]->getType()] = true; }
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
    // reset shaderprogram associated texture uniforms
    std::fill(this->shaderTextures.begin(), this->shaderTextures.end(), false);
    // default shader program
    types::ShaderProgram *defShader = collections::stored::StoredShaders::getStoredShader(core::StoredShaders::Diffuse);
    this->shaderTextures[types::Texture::Diffuse] = true;

    // guess by stored material textures

    if (hasTextureType[types::Texture::Specular]) {
        defShader = collections::stored::StoredShaders::getStoredShader(core::StoredShaders::Specular);
        this->shaderTextures[types::Texture::Specular] = true;
    }

    if (hasTextureType[types::Texture::Normals]) {
        defShader = collections::stored::StoredShaders::getStoredShader(core::StoredShaders::BumpedDiffuse);
        this->shaderTextures[types::Texture::Normals] = true;

        if (hasTextureType[types::Texture::Specular]) {
            defShader = collections::stored::StoredShaders::getStoredShader(core::StoredShaders::BumpedSpecular);
            this->shaderTextures[types::Texture::Specular] = true;
        }
    }

    setShaderProgram(defShader);
}

void types::Material::setTexturesUniforms()
{
    setTexturesUniforms(this->matShader);
}

void types::Material::setTexturesUniforms(types::ShaderProgram *shp)
{
    for (auto it = this->textures.begin(); it != this->textures.end(); it++) {
        // Obtain textype to query data info
        unsigned int texType = ((Texture *)*it)->getType();

        // only bind textures bound to the shader
        if (this->shaderTextures[texType]) {
            // bind texture
            ((Texture *)*it)->bind();
            // Set to the texture map shader the current texture assigned ID
            shp->setUniform(core::ShadersData::Samplers2D::NAMES[texType], (int)texType);
        }
    }
}

void types::Material::useMaterialShader()
{
    this->matShader->use();
}

types::Material::~Material(void)
{
    auto it = this->textures.begin();

    while (!this->textures.empty()) {
        collections::TexturesCollection::Instance()->unloadTexture((*it)->geTexId()) ? delete *it : 0;
        this->textures.erase(it++);
    }
}

void types::Material::loadMaterialValues(const aiMaterial *aiMat)
{
    aiColor4D tmpAmb(0.1f), tmpDiff(0.5f), tmpSpc(0.5f), tmpEmm(0.5);
    // query assimp material properties
    aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_AMBIENT, &tmpAmb);
    aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, &tmpDiff);
    aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_SPECULAR, &tmpSpc);
    aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_EMISSIVE, &tmpEmm);
    aiGetMaterialFloat(aiMat, AI_MATKEY_SHININESS, &shininess);
    aiGetMaterialInteger(aiMat, AI_MATKEY_SHADING_MODEL, &shadingModel);
    // Save properties to current material
    this->ambient = glm::vec3(tmpAmb.r, tmpAmb.g, tmpAmb.b);
    this->diffuse = glm::vec3(tmpDiff.r, tmpDiff.g, tmpDiff.b);
    this->specular = glm::vec3(tmpSpc.r, tmpSpc.g, tmpSpc.b);
    this->emission = glm::vec3(tmpEmm.r, tmpEmm.g, tmpEmm.b);
}