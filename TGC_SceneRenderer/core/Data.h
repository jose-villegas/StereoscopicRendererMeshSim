#pragma once
#include "..\Types\ShaderProgram.h"
namespace core {
    // Use shared_data.glsl for reference
    class ShadersData {
        public:
            struct Constrains {
                static const int MAX_LIGHTS = 8;
            };

            struct Structures {
                static const int LIGHT_MEMBER_COUNT = 8;
                static const int MATERIAL_MEMBER_COUNT = 4;
                static const int MATRIX_MEMBER_COUNT = 6;
                static const char *LIGHT_MEMBER_NAMES[];
                static const char *MATERIAL_MEMBER_NAMES[];
                static const char *MATRIX_MEMBER_NAMES[];
            };
            // Attachs predefined uniforms to the shader program
            static void AttachShaderData(types::ShaderProgram *shp);
    };

    class AvailableShaders {
        public:
            enum Shaders {
                Diffuse,
                Count // not a shader, represents the number of available shaders
            };
    };
}

