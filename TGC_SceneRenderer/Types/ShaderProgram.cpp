#include "ShaderProgram.h"
using namespace Types;

Types::ShaderProgram::ShaderProgram(void)
{
    this->_programID = glCreateProgram();
    this->_shaderCount = 0;

    if (this->_programID == 0) {
        Utils::Logger::Write("Error Creating Shader Program", false, LOG_CONTEXT_DANGER);
    }
}

Types::ShaderProgram::~ShaderProgram(void)
{
    glDeleteProgram(this->_programID);
}

void Types::ShaderProgram::attachShader(const Shader &pShader)
{
    glAttachShader(this->_programID, pShader.getId());
    this->_shaderCount++;
}

bool Types::ShaderProgram::link() const
{
    if (this->_shaderCount >= 2) {
        // Link Attached Shaders to Program
        glLinkProgram(this->_programID);
        // Check Linking Status
        GLint linkStatus;
        glGetProgramiv(this->_programID, GL_LINK_STATUS, &linkStatus);

        if (linkStatus == GL_FALSE) {
            Utils::Logger::Write("Shader Program Linking Failed", false, LOG_CONTEXT_DANGER);
            return false;
        }

        Utils::Logger::Write("Shader Program Linking Successful", false, LOG_CONTEXT_SUCCESS);
        return true;
    }

    return false;
}

void Types::ShaderProgram::use() const
{
    glUseProgram(this->_programID);
}

void Types::ShaderProgram::disable() const
{
    glUseProgram(0);
}

GLuint Types::ShaderProgram::getUniform(const std::string &sUniformName) const
{
    // Find uniform with this name
    std::unordered_map<std::string, GLuint>::const_iterator it = this->_uniformLoc.find(sUniformName);

    if (it != _uniformLoc.end()) {
        return it->second;
    } else {
        Utils::Logger::Write("Could not find uniform (" + sUniformName + ") in shader program", false, LOG_CONTEXT_DANGER);
        return -1;
    }
}

GLuint Types::ShaderProgram::addUniform(const std::string &sUniformName)
{
    // Try to obtain uniform location
    this->_uniformLoc[sUniformName] = glGetUniformLocation(this->_programID, sUniformName.c_str());

    // Check if an uniform with this name actually exists
    if (this->_uniformLoc[sUniformName] == -1) {
        Utils::Logger::Write("Could not add uniform: (" + sUniformName + ") location returned -1", false, LOG_CONTEXT_DANGER);
        return -1;
    }

    // Return Location
    Utils::Logger::Write("Uniform (" + sUniformName + ") bound to location: " , false, LOG_CONTEXT_SUCCESS);
    return this->_uniformLoc[sUniformName];
}

template<typename T>
void Types::ShaderProgram::setUniform(const std::string &sUniformName, T &&value) const
{
    GLuint unfrLoc = getUniform(name);

    if (unfrLoc == -1) { return; }

    setUniform(unfrLoc, std::forward<T>(value));
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const float value0) const
{
    glUniform1f(unfrLoc, value0);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const int value0) const
{
    glUniform1i(unfrLoc, value0);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const unsigned int value0) const
{
    glUniform1ui(unfrLoc, value0);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const glm::mat4 &value0) const
{
    glUniformMatrix4fv(unfrLoc, 1, GL_FALSE, glm::value_ptr(value0));
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const glm::mat3 &value0) const
{
    glUniformMatrix3fv(unfrLoc, 1, GL_FALSE, glm::value_ptr(value0));
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const glm::mat2 &value0) const
{
    glUniformMatrix2fv(unfrLoc, 1, GL_FALSE, glm::value_ptr(value0));
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const glm::vec4 &value0) const
{
    glUniform4fv(unfrLoc, 1, glm::value_ptr(value0));
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const glm::vec3 &value0) const
{
    glUniform3fv(unfrLoc, 1, glm::value_ptr(value0));
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const glm::vec2 &value0) const
{
    glUniform2fv(unfrLoc, 1, glm::value_ptr(value0));
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const float value0, const float value1) const
{
    glUniform2f(unfrLoc, value0, value1);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const float value0, const float value1, const float value2) const
{
    glUniform3f(unfrLoc, value0, value1, value2);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const float value0, const float value1, const float value2, const float value3) const
{
    glUniform4f(unfrLoc, value0, value1, value2, value3);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const int value0, const int value1) const
{
    glUniform2i(unfrLoc, value0, value1);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const int value0, const int value1, const int value2) const
{
    glUniform3i(unfrLoc, value0, value1, value2);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const int value0, const int value1, const int value2, const int value3) const
{
    glUniform4i(unfrLoc, value0, value1, value2, value3);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const unsigned int value0, const unsigned int value1) const
{
    glUniform2ui(unfrLoc, value0, value1);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const unsigned int value0, const unsigned int value1, const unsigned int value2) const
{
    glUniform3ui(unfrLoc, value0, value1, value2);
}

void Types::ShaderProgram::setUniform(unsigned int unfrLoc, const unsigned int value0, const unsigned int value1, const unsigned int value2, const unsigned int value3) const
{
    glUniform4ui(unfrLoc, value0, value1, value2, value3);
}

