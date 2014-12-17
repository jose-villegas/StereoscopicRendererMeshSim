#pragma once
#include "..\types\ShaderProgram.h"
namespace collections {
    namespace stored {

        class Shaders {
            private:
                static std::vector<types::ShaderProgram *> _shaders;
            public:

                enum DefaultShaders {
                    Diffuse,
                    BumpedDiffuse,
                    Count				// Not a Shader - Number of default shaders
                };

                static void loadShaders();
                static types::ShaderProgram *getDefaultShader(const DefaultShaders &value);
        };
    }
}

