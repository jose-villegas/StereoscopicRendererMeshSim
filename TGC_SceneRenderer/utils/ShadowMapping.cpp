#include "ShadowMapping.h"
#include "..\collections\MeshesCollection.h"

using namespace utils;

ShadowMapping::ShadowMapping(void) : shadowBias(0.5f), shadowMapSize(0), shadowMappingEnabled(true)
{
    this->depthRenderTexture  = nullptr;
    this->lightSource         = nullptr;
    this->matrices            = nullptr;
    this->shaderLinkProgram   = nullptr;
    this->shadowStrength	  = glm::vec3(0.15f);
    this->lightProjectorIndex = 0;
}

ShadowMapping::~ShadowMapping(void)
{
}

void utils::ShadowMapping::setup(const unsigned int shadowMapSize)
{
    // shadow mapping is already set up
    if (nullptr != this->depthRenderTexture) { return; }

    this->depthRenderTexture = new types::TextureRenderer();
    // create frame buffer object with specified render size bounds
    this->depthRenderTexture->createRenderTarget(shadowMapSize, shadowMapSize);
    // attach a depth map to the fbo, no color attachments only need depth
    this->depthRenderTexture->attachDepthTexture();
    // we are not going to render o read to / from any color buffer
    this->depthRenderTexture->bind();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    this->depthRenderTexture->unbind();
    // save shadow quality for later use
    this->shadowMapSize = shadowMapSize;
    // set shader program
    this->shaderLinkProgram = collections::stored::StoredShaders::getStoredShader(core::StoredShaders::Depth);
    // create matrices handler
    this->matrices = new core::Matrices();
    this->matrices->setShaderProgram(shaderLinkProgram);
    this->matrices->setUniformBlockInfo();
    // set uniform block info
    this->setUniformBlockInfo();
}

void utils::ShadowMapping::setup(const ShadowQualityPreset preset)
{
    this->setup((const unsigned int)preset);
}

void utils::ShadowMapping::setLightSource(scene::Light *lightSource)
{
    // invalid parameter or light source is already set
    if (nullptr == lightSource || nullptr != this->lightSource || nullptr == depthRenderTexture) { return; }

    // set class light handler
    this->lightSource = lightSource;
    this->lightProjectorIndex = this->lightSource->getShadowProjectorIndex();
    // camera, helper to render from light point of view
    this->lightPov = new scene::Camera();
}

void utils::ShadowMapping::bindMapping()
{
    if (nullptr == this->depthRenderTexture) { return; }

    this->depthRenderTexture->bind();
}

void utils::ShadowMapping::unbindMapping()
{
    if (nullptr == this->depthRenderTexture) { return; }

    this->depthRenderTexture->unbind();
}

void utils::ShadowMapping::projectShadowMap()
{
    if (nullptr == this->depthRenderTexture) { return; }

    // cull front faces to avoid self shadowing
    glCullFace(GL_FRONT);
    // bind render target
    this->depthRenderTexture->bind();
    // clear background color and buffers bits
    glClear(GL_DEPTH_BUFFER_BIT);
    // set viewport accordly to texture size
    this->lightPov->viewport(this->shadowMapSize, this->shadowMapSize);
    // set the camera view from light direction, only spot lights supported right now
    glm::vec3 lightDir = lightSource->getDirection();
    this->lightPov->base->transform.position = this->lightSource->base->transform.position;
    this->lightPov->base->transform.rotation = this->lightSource->base->transform.rotation;
    // adds extra degrees to avoid some flickering on the spot light outer angle borders
    float extraAngle = 2.f * glm::degrees(this->lightSource->outerConeAngle) / glm::pi<float>();
    this->lightPov->setFieldOfView(glm::degrees(this->lightSource->outerConeAngle) + extraAngle);
    this->matrices->setViewMatrix(this->lightPov->getViewMatrix());
    this->matrices->setProjectionMatrix(this->lightPov->getFrustumMatrix());
    // get meshes collection single instance
    collections::MeshesCollection *meshes = collections::MeshesCollection::Instance();
    // enable depth write shader
    this->shaderLinkProgram->use();

    // render all meshes with disabled textures and only position vertex atrib, we only need these for the depth value
    for (unsigned int i = 0; i < meshes->meshCount(); i++) {
        // set model view matrix per mesh
        this->matrices->setModelMatrix(meshes->getMesh(i)->base->transform.getModelMatrix());
        // recalculate matrices with current loaded matrices
        this->matrices->calculateMatrices();
        // update matrices uniform block data
        this->matrices->setUniformBlock();
        // finally call glDraw with mesh data, only use position vertex atrib and disable shaders
        // we don't need the rest because we are only querying depth info
        meshes->getMesh(i)->render(true, false, false, false, false, false);
    }

    // disable shader program
    this->shaderLinkProgram->disable();
    // restore to default render target, unbind current
    this->depthRenderTexture->unbind();
}

void utils::ShadowMapping::setUniformBlock()
{
    if (!this->uniformBlockInfo || !this->shaderLinkProgram) { return; }

    bindUniformBuffer();
    unsigned int line = this->lightProjectorIndex * core::ShadersData::Structures::SHADOW_MEMBER_COUNT;
    unsigned int memberCount = core::ShadersData::UniformBlocks::SHAREDSHADOWING_COMPLETE_COUNT;
    // copy actual values to uniform buffer memory positions
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[0 + line], &shadowMapSize, sizeof(unsigned int));
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[1 + line], glm::value_ptr(this->shadowStrength), sizeof(glm::vec3));
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[2 + line], glm::value_ptr(this->matrices->getModelViewProjection()), sizeof(glm::mat4));
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[memberCount - 2], &shadowMappingEnabled, sizeof(unsigned int));
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[memberCount - 1], &scene::Light::getShadowCount(), sizeof(unsigned int));
    updateUniformBufferData();
}

void utils::ShadowMapping::setUniformBlockInfo()
{
    this->uniformBlockInfo = this->shaderLinkProgram->getUniformBlock(core::ShadersData::UniformBlocks::SHAREDSHADOWING_NAME);

    // if uniform block info is null cancel
    if (this->uniformBlockInfo == nullptr) { return; }

    // set the uniform block info indices and offsets
    this->shaderLinkProgram->setUniformBlockInfoIndexAndOffset(
        this->uniformBlockInfo,
        core::ShadersData::UniformBlocks::SHAREDSHADOWING_COMPLETE_NAMES,
        core::ShadersData::UniformBlocks::SHAREDSHADOWING_COMPLETE_COUNT
    );
}

void utils::ShadowMapping::bindShadowMapTextures()
{
    this->depthRenderTexture->getDepthTexture()->setTextureType((unsigned int)(types::Texture::TextureType::Count + lightProjectorIndex));
    this->depthRenderTexture->getDepthTexture()->bind();
}

void utils::ShadowMapping::setTextureMapUniform(types::ShaderProgram *shp)
{
    shp->setUniform(core::ShadersData::Samplers::NAMES[types::Texture::TextureType::Count + 0], (int)this->depthRenderTexture->getDepthTexture()->getType());
}

const glm::mat4 utils::ShadowMapping::biasMatrix = glm::mat4(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        );
