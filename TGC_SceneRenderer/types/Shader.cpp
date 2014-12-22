#include "Shader.h"
using namespace types;

types::Shader::Shader(const ShaderType &shaderType)
{
    this->_type = shaderType;
    this->_id = glCreateShader(shaderType);
}

types::Shader::Shader(const ShaderType &shaderType, const std::string &source, const bool &loadFromFile /*= true*/)
{
    this->_type = shaderType;
    this->_id = glCreateShader(shaderType);
    loadFromFile ? this->loadFromFile(source) : this->loadFromString(source);
}

bool types::Shader::loadFromString(const std::string &sSource)
{
    if (sSource.empty()) { return false; }

    this->_source = sSource;
    const char *source = sSource.c_str();
    // Associate source with this shader ID
    glShaderSource(_id, 1, &source, NULL);
    // Successful shader file load
    std::cout << "Shader(" << this << "): " << getShaderTypeString() << " file " << _shaderName << " loaded successfully" << std::endl;
    // std::cout << std::endl << sSource << std::endl;
    return true;
}

bool types::Shader::loadFromString(std::string &sSource, const std::string &token, const std::string &data)
{
    // Find token at source code
    size_t glslTokenIndex = sSource.find(token);

    if (glslTokenIndex == std::string::npos) {
        std::cout << "Shader(" << this << "): " << "Token " << token << " not found in" << getShaderTypeString() << " source code" << std::endl;
        return loadFromString(sSource);
    }

    // Insert after token
    sSource.insert(glslTokenIndex + token.size(), data);
    std::cout << "Shader(" << this << "): " << getShaderTypeString() << " data modified \n " << data << "\ninserted at token " << token << std::endl;
    // Load normally
    loadFromString(sSource);
}

bool types::Shader::loadFromFile(const std::string &sFilename)
{
    // Convert the file to a string
    std::string source = fileToString(sFilename);

    if (source.empty()) {
        std::cout << "Shader(" << this << "): " << "Error Opening " << getShaderTypeString() << " file: " << sFilename << std::endl;
        return false;
    }

    _shaderName = sFilename;
    // Load Source and Associate with this shader ID
    return loadFromString(source);
}

bool types::Shader::loadFromFile(const std::string &sFilename, const std::string &token, const std::string &data)
{
    // Convert the file to a string
    std::string source = fileToString(sFilename);

    if (source.empty()) {
        std::cout << "Shader(" << this << "): " << "Error Opening " << getShaderTypeString() << " file: " << sFilename << std::endl;
        return false;
    }

    _shaderName = sFilename;
    // Load Source and Associate with this shader ID
    return loadFromString(source, token, data);
}


const std::string types::Shader::fileToString(const std::string &sFilename)
{
    std::ifstream file(sFilename, std::ifstream::in);

    if (!file.good()) {
        return std::string();
    }

    std::stringstream sourceSS;
    // Dump File Content into stream buffer
    sourceSS << file.rdbuf();
    file.close();
    // Return casted data
    return sourceSS.str();
}


bool types::Shader::compile()
{
    glCompileShader(_id);
    return compilationCheck();
}

bool types::Shader::compilationCheck()
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
        std::cout << "\n" << "Shader(" << this << "): " << getShaderTypeString() << " compilation errors:\n" << std::string(strInfoLog) << std::endl;
        // Free Reserved Memory for InfoLog
        delete[] strInfoLog;
        // Return Failure
        return false;
    } else {
        std::cout << "Shader(" << this << "): " << getShaderTypeString() << " file " << _shaderName << " compilation successful" << std::endl;
        return true;
    }
}

std::string types::Shader::getShaderTypeString()
{
    switch (_type) {
        case types::Shader::Vertex:
            return "Vertex shader";
            break;

        case types::Shader::Fragment:
            return "Fragment shader";
            break;

        case types::Shader::Geometry:
            return "Geometry shader";
            break;

        case types::Shader::TesselationControl:
            return "Tesselation control shader";
            break;

        case types::Shader::TesselationEvaluation:
            return "Tesselation evaluation shader";
            break;

        default:
            break;
    }
}