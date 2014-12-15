#include "Shader.h"
using namespace Types;

Types::Shader::Shader(const ShaderType shaderType)
{
    this->_type = shaderType;
    this->_id = glCreateShader(shaderType);
}

bool Types::Shader::loadFromString(const std::string &sSource)
{
    if (sSource.empty()) { return false; }

    this->_source = sSource;
    const char *source = sSource.c_str();
    // Associate source with this shader ID
    glShaderSource(_id, 1, &source, NULL);
    // Successful shader file load
    Utils::Logger::Write(getShaderTypeString() + " file: " + _shaderName + " loaded successfully", false, LOG_CONTEXT_SUCCESS);
    return true;
}

bool Types::Shader::loadFromFile(const std::string &sFilename)
{
    std::ifstream file(sFilename, std::ifstream::in);

    if (!file.good()) {
        Utils::Logger::Write("Error Opening " + getShaderTypeString() + " file: " + sFilename, true, LOG_CONTEXT_DANGER);
        return false;
    }

    std::stringstream sourceSS;
    // Dump File Content into stream buffer
    sourceSS << file.rdbuf();
    file.close();
    _shaderName = sFilename;
    // Load Source and Associate with this shader ID
    return loadFromString(sourceSS.str());
}

bool Types::Shader::compile()
{
    glCompileShader(_id);
    return compilationCheck();
}

bool Types::Shader::compilationCheck()
{
    GLint shaderStatus;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &shaderStatus);

    if (shaderStatus == GL_FALSE) {
        // Get Info Log Size
        GLint infoLength;
        glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &infoLength);
        // Save Info Log Data
        GLchar *strInfoLog = new GLchar[infoLength + 1];
        glGetShaderInfoLog(_id, infoLength, NULL, strInfoLog);
        // Write Compilation Errors to Utils Logger
        Utils::Logger::Write(std::string(strInfoLog), false, LOG_CONTEXT_DANGER);
        // Free Reserved Memory for InfoLog
        delete[] strInfoLog;
        // Return Failure
        return false;
    } else {
        Utils::Logger::Write(getShaderTypeString() + " file: " + _shaderName + " compilation successful", LOG_CONTEXT_SUCCESS);
        return true;
    }
}

std::string Types::Shader::getShaderTypeString()
{
    switch (_type) {
        case Types::Shader::Vertex:
            return "Vertex shader";
            break;

        case Types::Shader::Fragment:
            return "Fragment shader";
            break;

        case Types::Shader::Geometry:
            return "Geometry shader";
            break;

        case Types::Shader::TesselationControl:
            return "Tesselation control shader";
            break;

        case Types::Shader::TesselationEvaluation:
            return "Tesselation evaluation shader";
            break;

        default:
            break;
    }
}


