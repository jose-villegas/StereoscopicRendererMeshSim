#pragma once
#include "GL\gl_core_4_4.h"
#include "assimp\material.h"
#include "FreeImage\FreeImage.h"
#include <string>
namespace Types {
    class Texture {
        public:

            enum TextureType {
                None = aiTextureType_NONE,
                Diffuse = aiTextureType_DIFFUSE,
                Specular = aiTextureType_SPECULAR,
                Ambient = aiTextureType_AMBIENT,
                Emissive = aiTextureType_EMISSIVE,
                Height = aiTextureType_HEIGHT,
                Normals = aiTextureType_NORMALS,
                Shininess = aiTextureType_SHININESS,
                Ocapacity = aiTextureType_OPACITY,
                Displacement = aiTextureType_DISPLACEMENT,
                Lightmap = aiTextureType_LIGHTMAP,
                Reflection = aiTextureType_REFLECTION
            };

            Texture(void);
            Texture(std::string sFilename, const unsigned int texID, TextureType tType);
            ~Texture();
            TextureType tType;
            std::string sFilename;
            unsigned int tWidth;
            unsigned int tHeight;
            unsigned int tBitsPerPixel;
            unsigned int oglTexID;
            unsigned int texID;
            unsigned int internalFormat;
            unsigned int imageFormat;
            void bind() const;
            bool load(std::string sFilename);
            void unload() const;
    };
}
