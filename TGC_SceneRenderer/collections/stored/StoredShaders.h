#pragma once
#include "..\types\ShaderProgram.h"
#include "..\core\Data.h"
namespace collections {
    namespace stored {
        class StoredShaders {
            private:
                static std::vector<types::ShaderProgram *> shaders;
            public:
                static void LoadShaders();
                static types::ShaderProgram *getDefaultShader(const core::StoredShaders::Shaders &sh);
                static types::ShaderProgram *getDefaultShader(const std::string &shaderName);
        };
    }
}

