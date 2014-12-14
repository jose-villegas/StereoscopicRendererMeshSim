#pragma once
#include "Shader.h"
#include <unordered_map>
#include <string>
namespace Types {

    class ShaderProgram {
        private:
            GLuint _programID;
            GLuint _shaderCount;
            std::unordered_map<std::string, GLuint> _uniformLoc;
        public:
            ShaderProgram(void);
            ~ShaderProgram(void);
            void attachShader(Shader *pShader);
            bool link();
            void use();
            void disable();
            GLuint getUniform(const std::string sUniformName);
            GLuint addUniform(const std::string sUniformName);
    };
}

