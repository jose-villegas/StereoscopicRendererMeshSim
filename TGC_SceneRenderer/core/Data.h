#pragma once
#include "assimp\material.h"
#include "GL\gl_core_4_4.h"

#define TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

namespace core {

    class Data {
        private:
            static bool dataSet;
        public:
            static void Initialize();
            static void Clear();
    };
    // Execution general enviroment general info
    // OS dependent implementation
    class ExecutionInfo {

            friend void core::Data::Initialize();

        public:
            static const unsigned int AVAILABLE_CORES;
            static const std::string EXEC_DIR;
    };

    class EngineData {

        private:

            static bool anisotropicFilteringAvailable;
            static GLfloat maxAnisotropicFiltering;
            friend void core::Data::Initialize();

        public:

            static bool AnisotropicFilteringAvaible() { return anisotropicFilteringAvailable; }
            static float MaxAnisotropicFilteringAvaible() { return (float)maxAnisotropicFiltering; }

            class Commoms {
                public:
                    static const int DEFAULT_TEXTURE_ID = 0;
                    static const int INVALID_MATERIAL = -1;
                    static const int INVALID_VALUE = -1;
            };

            // Engine / Shaders Constrains
            class Constrains {
                public:
                    static const int MAX_LIGHTS = 32;
                    static const int MAX_SHADOWMAPS = 2;
            };
    };

    // Use shared_data.glsl for reference
    // Stores all the relevant shaders and some engine values
    // Inialize() ShadersData for non-const members
    class ShadersData {

            friend void core::Data::Initialize();

        private:
            static void CREATE_SHAREDLIGHTS_COMPLETE_NAMES(char *outNames[]);
        public:

            // Avaible ShadersData Structures
            class Structures {
                public:

                    static const int LIGHT_MEMBER_COUNT = 10;
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
                    static const int SHAREDLIGHTS_MEMBER_COUNT = 3;
                    static const int SHAREDLIGHTS_COMPLETE_COUNT = EngineData::Constrains::MAX_LIGHTS * Structures::LIGHT_MEMBER_COUNT + SHAREDLIGHTS_MEMBER_COUNT - 1;
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
                    // Ordered by TextureType
                    static const char *NAMES[];
                    static const char *DEFAULT_TEX_FILENAME;
            };
            // complete filename location for shared_data.glsl file string
            static const std::string Filename();
        private:
            static const char *FILENAME;
    };

    class StoredShaders {

            friend void core::Data::Initialize();

        public:
            enum Shaders {
                Diffuse,
                Specular,
                BumpedDiffuse,
                BumpedSpecular,
                OpacityDiffuse,
                OpacitySpecular,
                OpacityBumpedDiffuse,
                OpacityBumpedSpecular,
                Count // not a shader, represents the number of available shaders
            };

            static const std::string Filename(const Shaders &index, const unsigned int &type);
        private:
            static const char *FILENAMES[];
    };

    class StoredMeshes {

            friend void core::Data::Initialize();

        public:
            enum Meshes {
                Cube,
                Cylinder,
                Sphere,
                Torus,
                Count // not a mesh, it represents the enum size
            };

            static const char *NAMES[];
            static const std::string Filename(const unsigned int &index);
        private:
            static const char *FILENAMES[];
    };
}

