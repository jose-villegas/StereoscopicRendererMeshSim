#include "Matrices.h"
using namespace core;

Matrices::Matrices(void)
{
    view = modelView = model = modelViewProjection = projection = normal = glm::mat4(1.0f);
}

void core::Matrices::setUniformBlock()
{
    if (!this->uniformBlockInfo || !this->sLinkSP) { return; }

    // bind the uniform block as current
    bindUniformBuffer();
    // Copy values to buffer object memory addresses
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[0], glm::value_ptr(this->modelViewProjection), sizeof(glm::mat4));
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[1], glm::value_ptr(this->modelView), sizeof(glm::mat4));
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[2], glm::value_ptr(this->model), sizeof(glm::mat4));
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[3], glm::value_ptr(this->view), sizeof(glm::mat4));
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[4], glm::value_ptr(this->projection), sizeof(glm::mat4));
    memcpy(this->uniformBlockInfo->dataPointer + this->uniformBlockInfo->offset[5], glm::value_ptr(this->normal), sizeof(glm::mat4));
    // Update buffer data with the new data
    updateUniformBufferData();
}

void core::Matrices::calculateMatrices()
{
    this->modelView = this->view * this->model;
    this->modelViewProjection = this->projection * this->modelView;
    this->normal = glm::inverseTranspose(modelView);
}

void core::Matrices::setModelMatrix(const glm::mat4 &value)
{
    this->model = value;
}

void core::Matrices::setViewMatrix(const glm::mat4 &value)
{
    this->view = value;
}

void core::Matrices::setProjectionMatrix(const glm::mat4 &value)
{
    this->projection = value;
}

void core::Matrices::setUniformBlockInfo()
{
    this->uniformBlockInfo = this->sLinkSP->getUniformBlock(ShadersData::UniformBlocks::SHAREDMATRICES_NAME);

    // if uniform block info is null cancel
    if (this->uniformBlockInfo == nullptr) { return; }

    // set the uniform block info indices and offsets
    this->sLinkSP->setUniformBlockInfoIndexAndOffset(
        this->uniformBlockInfo,
        ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_NAMES,
        ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_COUNT
    );
}

core::Matrices::~Matrices()
{
    delete uniformBlockInfo;
}
