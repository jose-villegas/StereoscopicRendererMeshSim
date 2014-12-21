#include "Matrices.h"
using namespace core;

Matrices::Matrices(void)
{
    view = modelView = model = modelViewProjection = projection = normal = glm::mat4(1.0f);
    this->_uniformBlockInfo = nullptr;
    this->_utilitySHP = nullptr;
}

void core::Matrices::setUniforms(types::ShaderProgram *shp)
{
    types::ShaderProgram::UniformBlock *uniformBlock =  shp->getUniformBlock(ShadersData::UniformBlocks::SHAREDMATRICES_NAME);
    GLuint *indices = new GLuint[ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_COUNT];
    GLint *offset = new GLint[ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_COUNT];
    // bind the uniform block as current
    shp->bindUniformBlock(ShadersData::UniformBlocks::SHAREDMATRICES_NAME);
    // get the offsets and indices based on the uniform block member names
    shp->getUniformBlockIndexAndOffset(
        ShadersData::UniformBlocks::SHAREDMATRICES_NAME,
        ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_NAMES, &indices, &offset,
        ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_COUNT
    );
    // Copy values to buffer object memory addresses
    memcpy(uniformBlock->dataPointer + offset[0], glm::value_ptr(this->modelViewProjection), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[1], glm::value_ptr(this->modelView), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[2], glm::value_ptr(this->model), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[3], glm::value_ptr(this->view), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[4], glm::value_ptr(this->projection), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[5], glm::value_ptr(this->normal), sizeof(glm::mat4));
    // Update buffer data
    shp->updateUniformBlockBufferData(ShadersData::UniformBlocks::SHAREDMATRICES_NAME);
    // Delete reserved data
    delete[] indices;
    delete[] offset;
}

void core::Matrices::setUniforms()
{
    if (!this->_uniformBlockInfo) { return; }

    // bind the uniform block as current
    this->_utilitySHP->bindUniformBlock(ShadersData::UniformBlocks::SHAREDMATRICES_NAME);
    // Copy values to buffer object memory addresses
    memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[0], glm::value_ptr(this->modelViewProjection), sizeof(glm::mat4));
    memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[1], glm::value_ptr(this->modelView), sizeof(glm::mat4));
    memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[2], glm::value_ptr(this->model), sizeof(glm::mat4));
    memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[3], glm::value_ptr(this->view), sizeof(glm::mat4));
    memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[4], glm::value_ptr(this->projection), sizeof(glm::mat4));
    memcpy(this->_uniformBlockInfo->dataPointer + this->_uniformBlockInfo->offset[5], glm::value_ptr(this->normal), sizeof(glm::mat4));
    // Update buffer data
    this->_utilitySHP->updateUniformBlockBufferData(ShadersData::UniformBlocks::SHAREDMATRICES_NAME);
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
    this->_utilitySHP = collections::stored::Shaders::getDefaultShader(core::AvailableShaders::Diffuse);
    this->_uniformBlockInfo = this->_utilitySHP->getUniformBlock(ShadersData::UniformBlocks::SHAREDMATRICES_NAME);

    // if uniform block info is null cancel
    if (this->_uniformBlockInfo == nullptr) { return; }

    // Reserve memory for indices and offsets
    this->_uniformBlockInfo->indices = new GLuint[ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_COUNT];
    this->_uniformBlockInfo->offset = new GLint[ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_COUNT];
    // set the uniform block info indices and offsets
    this->_utilitySHP->setUniformBlockInfoIndexAndOffset(
        ShadersData::UniformBlocks::SHAREDMATRICES_NAME,
        this->_uniformBlockInfo,
        ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_NAMES,
        ShadersData::UniformBlocks::SHAREDMATRICES_MEMBER_COUNT
    );
}

core::Matrices::~Matrices()
{
    delete _uniformBlockInfo;
}
