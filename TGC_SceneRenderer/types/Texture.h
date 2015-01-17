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

            enum TextureWrappingMode {
                Repeat         = GL_REPEAT,
                ClampToEdge    = GL_CLAMP_TO_EDGE,
                MirroredRepeat = GL_MIRRORED_REPEAT,
            };

            Texture(const std::string &sFilename, const unsigned int &texId, const TextureType &tType);
            Texture(const unsigned int &texId, const TextureType &tType);
            Texture();
            ~Texture();
            // binds the texture to appropiate texture type
            // meaning GL_TEXTURE0 + textureType const int
            void bind() const;
            // uses class sFilename
            bool loadTexture();
            // loads texture from specified filepath
            bool loadTexture(const std::string &sFilename);
            // creates a texture based on passed parameters, saves the parameters to the texture object
            void createTexture(const unsigned int width, const unsigned int height,
                               const TextureFilteringMode min = Nearest,
                               const TextureFilteringMode mag = Nearest,
                               const TextureWrappingMode sWrap = Repeat,
                               const TextureWrappingMode tWrap = Repeat,
                               const bool generateMipmaps = false,
                               const unsigned int readType = GL_UNSIGNED_BYTE,
                               const GLint internalFormat = GL_RGB,
                               const GLint format = GL_RGB,
                               void *rawData = nullptr);
            // deletes texture from the GPU memory
            void unload() const;
            // sets the texture min and mag filtering flag
            void setFilteringMode(const TextureFilteringMode min, const TextureFilteringMode mag, const bool generateMipmaps);
            // sets wrapping mode
            void setWrappingMode(const TextureWrappingMode wrapS, const TextureWrappingMode wrapT);

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

        protected:

            // creates a texture based on class parameters input raw data
            void createTexture(void *rawData);
            // creates a texture based on class parameters
            void createTexture();

            static bool evaluateAnisoLevel(Texture *tex, const float level);

            TextureType textureType;

            TextureFilteringMode magFilteringMode;
            TextureFilteringMode minFilteringMode;
            TextureWrappingMode sWrappingMode;
            TextureWrappingMode tWrappingMode;

            // aniso level shared between all textures, 0 = disabled
            static float anisotropicFilteringLevel;

            unsigned int width;
            unsigned int height;
            unsigned int bitsPerPixel;
            unsigned int oglTexId;
            unsigned int texId;
            unsigned int textureDimension;
            unsigned int internalFormat;
            unsigned int format;
            unsigned int readType;
            // amount of material references, used to safe deletion if different
            // meshes share the same textures on different materials
            unsigned int referenceCount;
            bool generateMipmaps;
            bool enableAnisotropic;

            std::string sFilename;
    };
}
