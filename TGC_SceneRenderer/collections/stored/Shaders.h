#pragma once
#include "..\types\ShaderProgram.h"
#include "..\core\Data.h"
namespace collections {
    namespace stored {
        class Shaders {
            private:
                static std::vector<types::ShaderProgram *> _shaders;
            public:
                static void LoadShaders();
                static types::ShaderProgram *getDefaultShader(const core::AvailableShaders::Shaders &sh);
                static types::ShaderProgram *getDefaultShader(const std::string &shaderName);
        };
    }
}

