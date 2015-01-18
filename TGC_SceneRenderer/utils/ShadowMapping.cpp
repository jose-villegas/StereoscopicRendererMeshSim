#include "ShadowMapping.h"
using namespace utils;

ShadowMapping::ShadowMapping(void)
{
    this->depthRenderTexture = nullptr;
}

ShadowMapping::~ShadowMapping(void)
{
}

void utils::ShadowMapping::setup(const unsigned int shadowMapSize)
{
    // shadow mapping is already set up
    if (nullptr != this->depthRenderTexture) { return; }

    // create frame buffer object with specified render size bounds
    this->depthRenderTexture->createRenderTarget(shadowMapSize, shadowMapSize);
    // attach a depth map to the fbo, no color attachments only need depth
    this->depthRenderTexture->attachDepthTexture();
}

void utils::ShadowMapping::setup(const ShadowQualityPreset preset)
{
    this->setup((const unsigned int)preset);
}

void utils::ShadowMapping::setLightSource(scene::Light *lightSource)
{
    if (nullptr == lightSource) { return; }

    this->lightSource = lightSource;
}

void utils::ShadowMapping::bindMapping()
{
}

void utils::ShadowMapping::unbindMapping()
{
}
