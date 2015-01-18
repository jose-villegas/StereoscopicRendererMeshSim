#include "LightsCollection.h"
#include "..\Scene\Light.h"
#include "glm\detail\func_common.hpp"

using namespace collections;

LightsCollection::LightsCollection(void)
{
    this->viewMatrix = glm::mat4(1.0f);
}

LightsCollection *collections::LightsCollection::Instance()
{
    if (!instance) {
        instance = new LightsCollection();
    }

    return instance;
}

void collections::LightsCollection::setUniformBlock()
{
    if (!this->uniformBlockInfo || !this->shaderLinkProgram) { return; }

    bindUniformBuffer();
    unsigned int lightCount = glm::min((int)this->lights.size(), core::EngineData::Constrains::MAX_LIGHTS);

    for (unsigned int i = 0; i < lightCount; i++) {
        unsigned int line = i * core::ShadersData::Structures::LIGHT_MEMBER_COUNT;
        // convert position and direction to camera / eye space
        glm::vec4 lightPositionCameraSpace = viewMatrix * glm::vec4(this->lights[i]->base->transform.position, 1.0f);
        glm::vec4 lightDirectionCameraSpace = viewMatrix * glm::vec4(this->lights[i]->getDirection(), 0.0f);
        // copy actual values to uniform buffer memory positions
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[0 + line], glm::value_ptr(lightPositionCameraSpace), sizeof(glm::vec3));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[1 + line], glm::value_ptr(lightDirectionCameraSpace), sizeof(glm::vec3));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[2 + line], glm::value_ptr(lights[i]->color), sizeof(glm::vec3));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[3 + line], &lights[i]->intensity, sizeof(float));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[4 + line], &lights[i]->attenuation, sizeof(float));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[5 + line], &lights[i]->innerConeAngle, sizeof(float));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[6 + line], &lights[i]->outerConeAngle, sizeof(float));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[7 + line], &lights[i]->cosInnerConeAngle, sizeof(float));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[8 + line], &lights[i]->cosOuterConeAngle, sizeof(float));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[9 + line], &lights[i]->lightType, sizeof(int));
    }

    int sharedLightsCompleteCount = core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT;
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[sharedLightsCompleteCount - 2], glm::value_ptr(scene::Light::ambientLight), sizeof(glm::vec3));
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[sharedLightsCompleteCount - 1], &lightCount, sizeof(unsigned int));
    updateUniformBufferData();
}

void collections::LightsCollection::setUniformBlockInfo()
{
    this->uniformBlockInfo = this->shaderLinkProgram->getUniformBlock(core::ShadersData::UniformBlocks::SHAREDLIGHTS_NAME);

    // if uniform block info is null cancel
    if (this->uniformBlockInfo == nullptr) { return; }

    // set the uniform block info indices and offsets
    this->shaderLinkProgram->setUniformBlockInfoIndexAndOffset(
        this->uniformBlockInfo,
        (const char **)core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES,
        core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT
    );
}

void collections::LightsCollection::removeLight(const unsigned int &lightIndex)
{
    if (lightIndex >= lights.size()) { return; }

    // Delete index from vector
    this->lights.erase(lights.begin() + lightIndex);
}

void collections::LightsCollection::removeLight(scene::Light *lght)
{
    auto it = std::find(this->lights.begin(), this->lights.end(), lght);

    if (it == this->lights.end()) { return; }

    this->lights.erase(it);
}

scene::Light *collections::LightsCollection::getLight(const unsigned int &lightIndex) const
{
    if (lightIndex >= lights.size()) { return nullptr; }

    return lights[lightIndex];
}

const unsigned int collections::LightsCollection::lightCount() const
{
    return this->lights.size();
}

collections::LightsCollection::~LightsCollection()
{
    this->lights.clear();
}

scene::Light *collections::LightsCollection::createLight(const unsigned int lightType)
{
    if (this->lights.size() >= core::EngineData::Constrains::MAX_LIGHTS) { return nullptr; }

    this->lights.push_back(new scene::Light((scene::Light::LightType)lightType));
    // Return last added light
    return this->lights.back();
}

LightsCollection *collections::LightsCollection::instance;
