#include "Material.h"
#include "..\Collections\TexturesCollection.h"
#include "..\Scene\Light.h"

using namespace types;

Material::Material(void)
{
    ambient = diffuse = specular = glm::vec3(0.5f);
    this->shininess = 16.0f;
    this->emission = glm::vec3(0.5);
    this->matShader = nullptr;
    this->hasTextureType.resize((unsigned int)Texture::TextureType::Count);
    this->shaderTextures.resize((unsigned int)Texture::TextureType::Count);
    std::fill(this->hasTextureType.begin(), this->hasTextureType.end(), 0);
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

    tex->addReference();
    this->textures.push_back(tex);
    this->hasTextureType[(int)tex->getType()] = textures.size();
}

void types::Material::addTexture(Texture *tex, types::Texture::TextureType texType)
{
    // if texture is null, cancel
    if (!tex) { return; }

    tex->addReference();
    this->textures.push_back(tex);
    this->hasTextureType[texType] = textures.size();
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

    if (hasTextureType[types::Texture::Opacity]) {
        defShader = collections::stored::StoredShaders::getStoredShader(core::StoredShaders::OpacityDiffuse);
        this->shaderTextures[types::Texture::Opacity] = true;

        if (hasTextureType[types::Texture::Specular]) {
            defShader = collections::stored::StoredShaders::getStoredShader(core::StoredShaders::OpacitySpecular);
        }

        if (hasTextureType[types::Texture::Normals]) {
            defShader = collections::stored::StoredShaders::getStoredShader(core::StoredShaders::OpacityBumpedDiffuse);

            if (hasTextureType[types::Texture::Specular]) {
                defShader = collections::stored::StoredShaders::getStoredShader(core::StoredShaders::OpacityBumpedSpecular);
            }
        }
    }

    unsigned int i = 0;

    for (unsigned i = 0; i < shaderTextures.size(); i++) {
        if (shaderTextures[i]) {
            std::pair<unsigned int, Texture::TextureType> activePair(hasTextureType[i], (Texture::TextureType)i);
            this->activeShaderTextures.push_back(activePair);
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
    for (auto it = this->activeShaderTextures.begin(); it != this->activeShaderTextures.end(); it++) {
        Texture *texture = this->textures[it->first - 1];
        // Obtain textype to query data info
        unsigned int texType = it->second;
        // bind shader associated texture
        texture->bind();
        // Set to the texture map shader the current texture assigned ID
        shp->setUniform(core::ShadersData::Samplers::NAMES[texType], (int)texture->getType());
    }

    if (scene::Light::getShadowCount() > 0) {
        // get all the available shadow projectors, if shadowing is enabled we need to update the model matrix per model
        const std::array<utils::ShadowMapping *, core::EngineData::Constrains::MAX_SHADOWMAPS> &shadowProjectors = scene::Light::getShadowProjectors();

        // update model matrix per model and recalculate
        for (auto it = shadowProjectors.begin(); it != shadowProjectors.end(); it++) {
            // no shadow casting
            if (*it == nullptr) { continue; }

            (*it)->setTextureMapUniform(shp);
        }
    }
}

void types::Material::useMaterialShader()
{
    this->matShader->use();
}

types::Material::~Material(void)
{
    for (auto it = this->textures.begin(); it != this->textures.end(); it++) {
        if ((*it)->getReferenceCount() <= 1) {
            collections::TexturesCollection::Instance()->unloadTexture((*it)->geTexId()) ? delete *it : 0;
        } else { (*it)->removeReference(); }
    }

    this->textures.clear();
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