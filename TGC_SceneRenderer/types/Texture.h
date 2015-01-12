#pragma once
#include "../core/Data.h"
#include "assimp/material.h"
#include "FreeImage/FreeImage.h"
#include <string>

namespace types {
    class Texture {
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

            Texture(const std::string &sFilename, const unsigned int &texId, const TextureType &tType);
            Texture(const unsigned int &texId, const TextureType &tType);
            ~Texture();
            void bind() const;
            bool load();
            bool load(const std::string &sFilename);
            void unload() const;

            unsigned int getWidth()    const { return this->width;   };
            unsigned int getHeight()   const { return this->height;  };
            unsigned int getOGLTexId() const { return this->oglTexId; };
            unsigned int geTexId()     const { return this->texId;    };
            unsigned int getType()     const { return this->textureType; };
            std::string getTextureTypeString();

            std::string getFilename() { return this->sFilename; };

            friend bool operator <(const Texture &lhs, const Texture &rhs)
            {
                return lhs.textureType > rhs.textureType;
            }

        private:
            unsigned int width;
            unsigned int height;
            unsigned int bitsPerPixel;
            unsigned int oglTexId;
            unsigned int texId;
            TextureType textureType;
            std::string sFilename;
    };
}
