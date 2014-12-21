#include "LightsCollection.h"
#include "../core/Data.h"
#include "../Bases/BaseObject.h"

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
    unsigned int lightCount = this->_sceneLights.size();

    for (unsigned int i = 0; i < lightCount; i++) {
        unsigned int line = i * core::Data::Structures::LIGHT_MEMBER_COUNT;
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[0 + line], glm::value_ptr(_sceneLights[i]->transform.position), sizeof(glm::vec3));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[1 + line], glm::value_ptr(glm::mat3_cast(_sceneLights[i]->transform.rotation) * glm::vec3(0.0, -1.0, 0.0)),
               sizeof(glm::vec3));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[2 + line], glm::value_ptr(_sceneLights[i]->color), sizeof(glm::vec3));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[3 + line], &_sceneLights[i]->intensity, sizeof(float));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[4 + line], &_sceneLights[i]->attenuation, sizeof(float));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[5 + line], &_sceneLights[i]->innerConeAngle, sizeof(float));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[6 + line], &_sceneLights[i]->outerConeAngle, sizeof(float));
        memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[7 + line], &_sceneLights[i]->lightType, sizeof(int));
    }

    memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[core::Data::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT - 1], &lightCount, sizeof(unsigned int));
    updateUniformBufferData();
}

void collections::LightsCollection::setUniformBlockInfo()
{
    this->_uniformBlockInfo = this->_sLinkSP->getUniformBlock(core::Data::UniformBlocks::SHAREDLIGHTS_NAME);

    // if uniform block info is null cancel
    if (this->_uniformBlockInfo == nullptr) { return; }

    // Reserve memory for indices and offsets
    this->_uniformBlockInfo->indices = new GLuint[core::Data::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT];
    this->_uniformBlockInfo->offset = new GLint[core::Data::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT];
    // set the uniform block info indices and offsets
    this->_sLinkSP->setUniformBlockInfoIndexAndOffset(
        core::Data::UniformBlocks::SHAREDLIGHTS_NAME,
        this->_uniformBlockInfo,
        (const char **)core::Data::UniformBlocks::SHAREDLIGHTS_COMPLETE_NAMES,
        core::Data::UniformBlocks::SHAREDLIGHTS_COMPLETE_COUNT
    );
}

scene::Light *collections::LightsCollection::createLight()
{
    this->_sceneLights.push_back(new scene::Light());
    // Return last added light
    return this->_sceneLights.back();
}

void collections::LightsCollection::removeLight(const unsigned int &lightIndex)
{
    if (lightIndex >= _sceneLights.size()) { return; }

    // Free memory reserved by light structure
    delete this->_sceneLights[lightIndex];
    // Delete index from vector
    this->_sceneLights.erase(_sceneLights.begin() + lightIndex);
}

scene::Light *collections::LightsCollection::getLight(const unsigned int &lightIndex) const
{
    if (lightIndex >= _sceneLights.size()) { return nullptr; }

    return _sceneLights[lightIndex];
}

const unsigned int collections::LightsCollection::lightCount() const
{
    return this->_sceneLights.size();
}

LightsCollection *collections::LightsCollection::_lightsCollection;
