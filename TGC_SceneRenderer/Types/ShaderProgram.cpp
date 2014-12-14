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

void Types::ShaderProgram::attachShader(Shader *pShader)
{
    glAttachShader(this->_programID, pShader->getId());
    this->_shaderCount++;
}

bool Types::ShaderProgram::link()
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

void Types::ShaderProgram::use()
{
    glUseProgram(this->_programID);
}

void Types::ShaderProgram::disable()
{
    glUseProgram(0);
}

GLuint Types::ShaderProgram::getUniform(const std::string sUniformName)
{
    // Find uniform with this name
    std::unordered_map<std::string, GLuint>::const_iterator it = this->_uniformLoc.find(sUniformName);

    if (it != _uniformLoc.end()) {
        return this->_uniformLoc[sUniformName];
    } else {
        Utils::Logger::Write("Could not find uniform: " + sUniformName + " in shader program:", false, LOG_CONTEXT_DANGER);
        return -1;
    }
}

GLuint Types::ShaderProgram::addUniform(const std::string sUniformName)
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
