#pragma once
#include "assimp\material.h"
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
                static const char *MATRIX_UNIFORMBLOCK_NAME;
                static const char *MATRIX_UNIFORMBLOCK_INSTANCE_NAME;
            };

            struct Samplers2D {
                enum TextureType {
                    None         = aiTextureType_NONE,
                    Diffuse      = aiTextureType_DIFFUSE,
                    Specular     = aiTextureType_SPECULAR,
                    Ambient      = aiTextureType_AMBIENT,
                    Emissive     = aiTextureType_EMISSIVE,
                    Height       = aiTextureType_HEIGHT,
                    Normals      = aiTextureType_NORMALS,
                    Shininess    = aiTextureType_SHININESS,
                    Ocapacity    = aiTextureType_OPACITY,
                    Displacement = aiTextureType_DISPLACEMENT,
                    Lightmap     = aiTextureType_LIGHTMAP,
                    Reflection   = aiTextureType_REFLECTION
                };
                // Ordered by TextureType
                static const char *NAMES[];
            };

            // Attachs predefined uniforms to the shader program
            static void AddShaderData(types::ShaderProgram *shp);
    };

    class AvailableShaders {
        public:
            enum Shaders {
                Diffuse,
                Count // not a shader, represents the number of available shaders
            };
    };
}

