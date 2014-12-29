#include "LightsCollection.h"

using namespace collections;

LightsCollection::LightsCollection(void)
{
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
    if (!this->uniformBlockInfo || !this->sLinkSP) { return; }

    bindUniformBuffer();
    unsigned int lightCount = this->lights.size();

    for (unsigned int i = 0; i < lightCount; i++) {
        unsigned int line = i * core::ShadersData::Structures::LIGHT_MEMBER_COUNT;
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[0 + line], glm::value_ptr(lights[i]->base->transform.position), sizeof(glm::vec3));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[1 + line], glm::value_ptr(glm::mat3_cast(lights[i]->base->transform.rotation) * glm::vec3(0.0, -1.0, 0.0)),
               sizeof(glm::vec3));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[2 + line], glm::value_ptr(lights[i]->color), sizeof(glm::vec3));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[3 + line], &lights[i]->intensity, sizeof(float));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[4 + line], &lights[i]->attenuation, sizeof(float));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[5 + line], &lights[i]->innerConeAngle, sizeof(float));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[6 + line], &lights[i]->outerConeAngle, sizeof(float));
        memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[7 + line], &lights[i]->lightType, sizeof(int));
    }

    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT - 1], &lightCount, sizeof(unsigned int));
    updateUniformBufferData();
}

void collections::LightsCollection::setUniformBlockInfo()
{
    this->uniformBlockInfo = this->sLinkSP->getUniformBlock(core::ShadersData::UniformBlocks::SHAREDLIGHTS_NAME);

    // if uniform block info is null cancel
    if (this->uniformBlockInfo == nullptr) { return; }

    // set the uniform block info indices and offsets
    this->sLinkSP->setUniformBlockInfoIndexAndOffset(
        this->uniformBlockInfo,
        (const char **)core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES,
        core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT
    );
}

scene::Light *collections::LightsCollection::createLight()
{
    this->lights.push_back(new scene::Light());
    // Return last added light
    return this->lights.back();
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

LightsCollection *collections::LightsCollection::instance;
