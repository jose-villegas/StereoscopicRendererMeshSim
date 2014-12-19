#include "ShaderProgram.h"

using namespace types;

types::ShaderProgram::ShaderProgram(void)
{
    this->_programID = glCreateProgram();
    this->_shaderCount = 0;

    if (this->_programID <= 0) {
        std::cout << "ShaderProgram(" << this << "): " << "Error Creating Shader Program" << std::endl;
    } else {
        std::cout << "ShaderProgram(" << this << "): " << "Shader Program " << _programID << " created successfully" << std::endl;
    }
}

types::ShaderProgram::~ShaderProgram(void)
{
    glDeleteProgram(this->_programID);
}

void types::ShaderProgram::attachShader(Shader *pShader)
{
    glAttachShader(this->_programID, pShader->getId());
    _attachedShaders.push_back(pShader);
    this->_shaderCount++;
}

bool types::ShaderProgram::link() const
{
    if (this->_shaderCount >= 2) {
        // Link Attached Shaders to Program
        glLinkProgram(this->_programID);
        // Check Linking Status
        GLint linkStatus;
        glGetProgramiv(this->_programID, GL_LINK_STATUS, &linkStatus);

        if (linkStatus == GL_FALSE) {
            std::cout << "ShaderProgram(" << this << "): " << "Shader program linking failed" << std::endl;
            return false;
        }

        std::cout << "ShaderProgram(" << this << "): " << "Shader program linking successful" << std::endl;
        return true;
    }

    return false;
}

void types::ShaderProgram::use() const
{
    glUseProgram(this->_programID);
}

void types::ShaderProgram::disable() const
{
    glUseProgram(0);
}

GLuint types::ShaderProgram::getUniform(const std::string &sUniformName) const
{
    // Find uniform with this name
    std::unordered_map<std::string, GLuint>::const_iterator it = this->_uniformLoc.find(sUniformName);

    if (it != _uniformLoc.end()) {
        return it->second;
    } else {
        return -1;
    }
}

GLubyte *types::ShaderProgram::getUniformBlock(const std::string &sUniformBlockName) const
{
    // Find uniform with this name
    std::unordered_map<std::string, GLubyte *>::const_iterator it = this->_uniformBlocks.find(sUniformBlockName);

    if (it != _uniformBlocks.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

GLuint types::ShaderProgram::addUniform(const std::string &sUniformName)
{
    // Try to obtain uniform location
    GLint nUniformLoc = glGetUniformLocation(this->_programID, sUniformName.c_str());

    // Check if an uniform with this name actually exists
    if (nUniformLoc == -1) {
        std::cout << "ShaderProgram(" << this << "): " << "Could not add uniform: (" << sUniformName << ") location returned -1" << std::endl;
        return -1;
    }

    // Return Location
    this->_uniformLoc[sUniformName] = nUniformLoc;
    std::cout << "ShaderProgram(" << this << "): " << "Uniform (" << sUniformName << ") bound to location: " << std::to_string(nUniformLoc) << std::endl;
    return nUniformLoc;
}

unsigned int types::ShaderProgram::addUniformBlock(const std::string &sUniformBlockName)
{
    auto it  = this->_uniformBlocks.find(sUniformBlockName);

    // There is a uniform block with this name already saved
    if (it != this->_uniformBlocks.end()) { return 1; }

    // Query block index
    GLuint blockIndex = glGetUniformBlockIndex(this->_programID, sUniformBlockName.c_str());

    // No block uniform with this name
    if (blockIndex == -1) { return -1; }

    // Query block uniform block size
    GLint blockSize;
    glGetActiveUniformBlockiv(this->_programID, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    GLubyte *blockBuffer = (GLubyte *)malloc(blockSize);
    // Store pointer to block buffer in uniformBlocks map
    this->_uniformBlocks[sUniformBlockName] = blockBuffer;
    // Create Buffer Object
    GLuint UB;
    glGenBuffers(1, &UB);
    glBindBuffer(GL_UNIFORM_BUFFER, UB);
    glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);
    // Bind the buffer
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, UB);
    // Return uniform buffer id
    return UB;
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const float &value0) const
{
    glUniform1f(uniformLocation, value0);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const int &value0) const
{
    glUniform1i(uniformLocation, value0);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const unsigned int &value0) const
{
    glUniform1ui(uniformLocation, value0);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const glm::mat4 &value0) const
{
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value0));
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const glm::mat3 &value0) const
{
    glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value0));
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const glm::mat2 &value0) const
{
    glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value0));
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const glm::vec4 &value0) const
{
    glUniform4fv(uniformLocation, 1, glm::value_ptr(value0));
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const glm::vec3 &value0) const
{
    glUniform3fv(uniformLocation, 1, glm::value_ptr(value0));
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const glm::vec2 &value0) const
{
    glUniform2fv(uniformLocation, 1, glm::value_ptr(value0));
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const float &value0, const float &value1) const
{
    glUniform2f(uniformLocation, value0, value1);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const float &value0, const float &value1, const float &value2) const
{
    glUniform3f(uniformLocation, value0, value1, value2);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const float &value0, const float &value1, const float &value2, const float &value3) const
{
    glUniform4f(uniformLocation, value0, value1, value2, value3);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const int &value0, const int &value1) const
{
    glUniform2i(uniformLocation, value0, value1);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const int &value0, const int &value1, const int &value2) const
{
    glUniform3i(uniformLocation, value0, value1, value2);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const int &value0, const int &value1, const int &value2, const int &value3) const
{
    glUniform4i(uniformLocation, value0, value1, value2, value3);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const unsigned int &value0, const unsigned int &value1) const
{
    glUniform2ui(uniformLocation, value0, value1);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const unsigned int &value0, const unsigned int &value1, const unsigned int &value2) const
{
    glUniform3ui(uniformLocation, value0, value1, value2);
}

void types::ShaderProgram::setUniform(unsigned int uniformLocation, const unsigned int &value0, const unsigned int &value1, const unsigned int &value2, const unsigned int &value3) const
{
    glUniform4ui(uniformLocation, value0, value1, value2, value3);
}

void types::ShaderProgram::getUniformBlockIndexAndOffset(const std::string &uniformBlockName, const char *names[], GLuint *outIndices, GLint *outOffset) const
{
    auto it = this->_uniformBlocks.find(uniformBlockName);

    // No uniform block with this name
    if (it == this->_uniformBlocks.end()) { return; }

    glGetUniformIndices(this->_programID, 4, names, outIndices);
    glGetActiveUniformsiv(this->_programID, 4, outIndices, GL_UNIFORM_OFFSET, outOffset);
}

std::unordered_map<std::string, GLubyte *> types::ShaderProgram::_uniformBlocks;
