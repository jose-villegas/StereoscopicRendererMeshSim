#include "LightsCollection.h"

using namespace collections;

LightsCollection::LightsCollection(void)
{
}

LightsCollection *collections::LightsCollection::Instance()
{
    if (!_lightsCollection) {
        _lightsCollection = new LightsCollection();
    }

    return _lightsCollection;
}

void collections::LightsCollection::setUniformBlock()
{
    if (!this->_uniformBlockInfo || !this->_sLinkSP) { return; }

    bindUniformBuffer();
    unsigned int lightCount = this->_lights.size();

    for (unsigned int i = 0; i < lightCount; i++) {
        unsigned int line = i * core::ShadersData::Structures::LIGHT_MEMBER_COUNT;
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[0 + line], glm::value_ptr(_lights[i]->base->transform.position), sizeof(glm::vec3));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[1 + line], glm::value_ptr(glm::mat3_cast(_lights[i]->base->transform.rotation) * glm::vec3(0.0, -1.0, 0.0)),
               sizeof(glm::vec3));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[2 + line], glm::value_ptr(_lights[i]->color), sizeof(glm::vec3));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[3 + line], &_lights[i]->intensity, sizeof(float));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[4 + line], &_lights[i]->attenuation, sizeof(float));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[5 + line], &_lights[i]->innerConeAngle, sizeof(float));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[6 + line], &_lights[i]->outerConeAngle, sizeof(float));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[7 + line], &_lights[i]->lightType, sizeof(int));
    }

    memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT - 1], &lightCount, sizeof(unsigned int));
    updateUniformBufferData();
}

void collections::LightsCollection::setUniformBlockInfo()
{
    this->_uniformBlockInfo = this->_sLinkSP->getUniformBlock(core::ShadersData::UniformBlocks::SHAREDLIGHTS_NAME);

    // if uniform block info is null cancel
    if (this->_uniformBlockInfo == nullptr) { return; }

    // Reserve memory for indices and offsets
    this->_uniformBlockInfo->indices = new GLuint[core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT];
    this->_uniformBlockInfo->offset = new GLint[core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT];
    // set the uniform block info indices and offsets
    this->_sLinkSP->setUniformBlockInfoIndexAndOffset(
        core::ShadersData::UniformBlocks::SHAREDLIGHTS_NAME,
        this->_uniformBlockInfo,
        (const char **)core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES,
        core::ShadersData::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT
    );
}

scene::Light *collections::LightsCollection::createLight()
{
    this->_lights.push_back(new scene::Light());
    // Return last added light
    return this->_lights.back();
}

void collections::LightsCollection::removeLight(const unsigned int &lightIndex)
{
    if (lightIndex >= _lights.size()) { return; }

    // Free memory reserved by light structure
    delete this->_lights[lightIndex];
    // Delete index from vector
    this->_lights.erase(_lights.begin() + lightIndex);
}

scene::Light *collections::LightsCollection::getLight(const unsigned int &lightIndex) const
{
    if (lightIndex >= _lights.size()) { return nullptr; }

    return _lights[lightIndex];
}

const unsigned int collections::LightsCollection::lightCount() const
{
    return this->_lights.size();
}

LightsCollection *collections::LightsCollection::_lightsCollection;
