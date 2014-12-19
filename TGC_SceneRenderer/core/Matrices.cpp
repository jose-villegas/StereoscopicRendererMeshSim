#include "Matrices.h"
using namespace core;

Matrices::Matrices(void)
{
    view = modelView = model = modelViewProjection = projection = normal = glm::mat4(1.0f);
}

void core::Matrices::setUniforms(types::ShaderProgram *shp)
{
    types::ShaderProgram::UniformBlock *uniformBlock =  shp->getUniformBlock(ShadersData::Structures::MATRIX_UNIFORMBLOCK_NAME);
    GLuint *indices = new GLuint[ShadersData::Structures::MATRIX_MEMBER_COUNT];
    GLint *offset = new GLint[ShadersData::Structures::MATRIX_MEMBER_COUNT];
    // bind the uniform block as current
    shp->bindUniformBlock(ShadersData::Structures::MATRIX_UNIFORMBLOCK_NAME);
    // get the offsets and indices based on the uniform block member names
    shp->getUniformBlockIndexAndOffset(
        ShadersData::Structures::MATRIX_UNIFORMBLOCK_NAME,
        ShadersData::Structures::MATRIX_MEMBER_NAMES, &indices, &offset,
        ShadersData::Structures::MATRIX_MEMBER_COUNT
    );
    // Copy values to buffer object memory addresses
    memcpy(uniformBlock->dataPointer + offset[0], glm::value_ptr(this->modelViewProjection), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[1], glm::value_ptr(this->modelView), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[2], glm::value_ptr(this->model), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[3], glm::value_ptr(this->view), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[4], glm::value_ptr(this->projection), sizeof(glm::mat4));
    memcpy(uniformBlock->dataPointer + offset[5], glm::value_ptr(this->normal), sizeof(glm::mat4));
    // Update buffer data
    shp->updateUniformBlockBufferData(ShadersData::Structures::MATRIX_UNIFORMBLOCK_NAME);
    // Delete reserved data
    delete indices;
    delete offset;
}

void core::Matrices::setUniforms()
{
    if (!this->_uniformBlockInfo) { return; }

    GLuint *indices = new GLuint[ShadersData::Structures::MATRIX_MEMBER_COUNT];
    GLint *offset = new GLint[ShadersData::Structures::MATRIX_MEMBER_COUNT];
    // bind the uniform block as current
    this->utilitySHP.bindUniformBlock(ShadersData::Structures::MATRIX_UNIFORMBLOCK_NAME);
    // get the offsets and indices based on the uniform block member names
    this->utilitySHP.getUniformBlockIndexAndOffset(
        ShadersData::Structures::MATRIX_UNIFORMBLOCK_NAME,
        ShadersData::Structures::MATRIX_MEMBER_NAMES, &indices, &offset,
        ShadersData::Structures::MATRIX_MEMBER_COUNT
    );
    // Copy values to buffer object memory addresses
    memcpy(this->_uniformBlockInfo->dataPointer + offset[0], glm::value_ptr(this->modelViewProjection), sizeof(glm::mat4));
    memcpy(this->_uniformBlockInfo->dataPointer + offset[1], glm::value_ptr(this->modelView), sizeof(glm::mat4));
    memcpy(this->_uniformBlockInfo->dataPointer + offset[2], glm::value_ptr(this->model), sizeof(glm::mat4));
    memcpy(this->_uniformBlockInfo->dataPointer + offset[3], glm::value_ptr(this->view), sizeof(glm::mat4));
    memcpy(this->_uniformBlockInfo->dataPointer + offset[4], glm::value_ptr(this->projection), sizeof(glm::mat4));
    memcpy(this->_uniformBlockInfo->dataPointer + offset[5], glm::value_ptr(this->normal), sizeof(glm::mat4));
    // Update buffer data
    this->utilitySHP.updateUniformBlockBufferData(ShadersData::Structures::MATRIX_UNIFORMBLOCK_NAME);
    // Delete reserved data
    delete indices;
    delete offset;
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

void core::Matrices::setUniformBlockInfo(types::ShaderProgram::UniformBlock *uniformBlockInfo)
{
    this->_uniformBlockInfo = uniformBlockInfo;
}

void core::Matrices::setUniformBlockInfoIndicesAndOffsets()
{
    if (!this->_uniformBlockInfo) { return; }
}
