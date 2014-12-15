#include "Shader.h"
using namespace Types;

Shader::Shader(void)
{
}

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
    return true;
}

bool Types::Shader::loadFromFile(const std::string &sFilename)
{
    std::ifstream file(sFilename, std::ifstream::in);

    if (!file.good()) {
        return false;
    }

    std::stringstream sourceSS;
    // Dump File Content into stream buffer
    sourceSS << file.rdbuf();
    file.close();
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
        Utils::Logger::Write(_shaderName + getShaderTypeString() + " compilation successful");
        return true;
    }
}

std::string Types::Shader::getShaderTypeString()
{
    switch (_type) {
        case Types::Shader::VERTEX_SHADER:
            return "Vertex Shader";
            break;

        case Types::Shader::FRAGMENT_SHADER:
            return "Fragment Shader";
            break;

        case Types::Shader::GEOMETRY_SHADER:
            return "Geometry Shader";
            break;

        case Types::Shader::TESSELATION_CONTROL:
            return "Tesselation Control Shader";
            break;

        case Types::Shader::TESSELATION_EVALUATION:
            return "Tesselation Evaluation Shader";
            break;

        default:
            break;
    }
}