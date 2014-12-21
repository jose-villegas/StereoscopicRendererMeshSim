#pragma once
#include "GL/gl_core_4_4.h"
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

            unsigned int getWidth()    const { return this->tWidth;   };
            unsigned int getHeight()   const { return this->tHeight;  };
            unsigned int getOGLTexId() const { return this->oglTexId; };
            unsigned int geTexId()     const { return this->texId;    };
            unsigned int getType()     const { return this->tType;    };

            std::string getFilename() { return this->sFilename; };

            friend bool operator <(const Texture &lhs, const Texture &rhs)
            {
                return lhs.tType > rhs.tType;
            }

        private:
            unsigned int tWidth;
            unsigned int tHeight;
            unsigned int tBitsPerPixel;
            unsigned int oglTexId;
            unsigned int texId;
            TextureType tType;
            std::string sFilename;
    };
}
