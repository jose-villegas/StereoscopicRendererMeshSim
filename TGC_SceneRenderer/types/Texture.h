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
                Opacity      = aiTextureType_OPACITY,
                Displacement = aiTextureType_DISPLACEMENT,
                Lightmap     = aiTextureType_LIGHTMAP,
                Reflection   = aiTextureType_REFLECTION,
                Count		 = 12
            };

            enum TextureFilteringMode {
                Nearest              = GL_NEAREST,
                Linear               = GL_LINEAR,
                NearestMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
                LinearMipmapNearest  = GL_LINEAR_MIPMAP_NEAREST,
                NearestMipmapLinear  = GL_NEAREST_MIPMAP_LINEAR,
                LinearMipmapLinear   = GL_LINEAR_MIPMAP_LINEAR,
            };

            Texture(const std::string &sFilename, const unsigned int &texId, const TextureType &tType);
            Texture(const unsigned int &texId, const TextureType &tType);
            ~Texture();
            // binds the texture to appropiate texture type
            // meaning GL_TEXTURE0 + textureType const int
            void bind() const;
            bool load();
            bool load(const std::string &sFilename);
            void unload() const;
            void setFilteringMode(const TextureFilteringMode min, const TextureFilteringMode mag);

            unsigned int getWidth()    const { return this->width;   }
            unsigned int getHeight()   const { return this->height;  }
            unsigned int getOGLTexId() const { return this->oglTexId; }
            unsigned int geTexId()     const { return this->texId;    }
            unsigned int getType()     const { return this->textureType; }
            unsigned int getReferenceCount() const { return this->referenceCount; }
            // adds one material reference to total references count
            void addReference() { this->referenceCount++; }
            // removes one material reference to total references count
            void removeReference() { this->referenceCount--; }

            std::string getTextureTypeString();
            std::string getFilename() { return this->sFilename; };
            // sets if this texture uses anisotropic filtering returns
            // false if anistropic evaluation failed otherwise returns val
            // warning: unbinds current binded texture
            bool enableAnisotropicFiltering(bool val);

            static void setAnisotropicFilteringLevel(const float level);

            friend bool operator <(const Texture &lhs, const Texture &rhs)
            {
                return lhs.textureType > rhs.textureType;
            }

        private:
            static bool evaluateAnisoLevel(Texture *tex, const float level);

            TextureType textureType;

            TextureFilteringMode magFilteringMode;
            TextureFilteringMode minFilteringMode;

            // aniso level shared between all textures, 0 = disabled
            static float anisotropicFilteringLevel;

            unsigned int width;
            unsigned int height;
            unsigned int bitsPerPixel;
            unsigned int oglTexId;
            unsigned int texId;
            // amount of material references, used to safe deletion if different
            // meshes share the same textures on different materials
            unsigned int referenceCount;
            bool generateMipmaps;
            bool enableAnisotropic;

            std::string sFilename;
    };
}
