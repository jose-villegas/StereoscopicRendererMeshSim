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
                Vertex = GL_VERTEX_SHADER,
                Fragment = GL_FRAGMENT_SHADER,
                Geometry = GL_GEOMETRY_SHADER,
                TesselationControl = GL_TESS_CONTROL_SHADER,
                TesselationEvaluation = GL_TESS_EVALUATION_SHADER
            };
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

