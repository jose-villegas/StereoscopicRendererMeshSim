#pragma once
#include "GL\gl_core_4_4.h"
#include "..\Utils\Logger.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>

namespace Types {

    class Shader {
        public:
            enum ShaderType {
                VERTEX_SHADER = GL_VERTEX_SHADER,
                FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
                GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
                TESSELATION_CONTROL = GL_TESS_CONTROL_SHADER,
                TESSELATION_EVALUATION = GL_TESS_EVALUATION_SHADER
            };
            Shader(void);
            Shader(const ShaderType shaderType);
            bool loadFromString(const std::string &sSource);
            bool loadFromFile(const std::string &sFilename);
            bool compile();
            GLuint getId() const { return _id; }
        private:
            GLuint _id;
            ShaderType _type;
            std::string _source;
            std::string _shaderName;
            bool compilationCheck();
            std::string getShaderTypeString();
    };
}

