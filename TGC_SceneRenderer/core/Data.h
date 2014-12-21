#pragma once
#include "assimp\material.h"
#include "..\Types\ShaderProgram.h"
namespace core {
    // Use shared_data.glsl for reference
    // Stores all the relevant shaders and engine values
    // Inialize() ShadersData for non-const members
    class Data {
        private:
            static bool dataSet;
            static void CREATE_SHAREDLIGHTS_COMPLETE_NAMES(char *outNames[]);
        public:
            // Inialize() ShadersData for non-const members
            static void Initialize();

            // Engine / Shaders Constrains
            class Constrains {
                public:
                    static const int MAX_LIGHTS = 8;
                    static const int MAX_SHADOWMAPS = 2;
            };
            // Avaible ShadersData Structures
            class Structures {
                public:

                    static const int LIGHT_MEMBER_COUNT = 8;
                    static const GLchar *LIGHT_MEMBER_NAMES[];
                    static const int MATERIAL_MEMBER_COUNT = 4;
                    static const GLchar *MATERIAL_MEMBER_NAMES[];
            };

            // Usual uniforms
            class Uniforms {
                public:
                    static const char *MATERIAL_INSTANCE_NAME;
            };

            // Shaders Uniform Blocks
            class UniformBlocks {
                public:
                    // SharedLight Uniform Block Info
                    static const int SHAREDLIGHTS_MEMBER_COUNT = 2;
                    static const int SHAREDLIGHTS_COMPLETE_COUNT = Constrains::MAX_LIGHTS *Structures::LIGHT_MEMBER_COUNT + 1;
                    static const GLchar *SHAREDLIGHTS_MEMBER_NAMES[];
                    static GLchar *SHAREDLIGHTS_COMPLETE_NAMES[SHAREDLIGHTS_COMPLETE_COUNT]; // Inialize() ShadersData for non-const members
                    static const char *SHAREDLIGHTS_NAME;
                    static const char *SHAREDLIGHTS_INSTANCE_NAME;
                    // SharedMatrices Uniform Block Info
                    static const int SHAREDMATRICES_MEMBER_COUNT = 6;
                    static const GLchar *SHAREDMATRICES_MEMBER_NAMES[];
                    static const char *SHAREDMATRICES_NAME;
                    static const char *SHAREDMATRICES_INSTANCE_NAME;

            };

            class Samplers2D {
                public:
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

            static const char *SHADER_NAMES[];
    };
}

