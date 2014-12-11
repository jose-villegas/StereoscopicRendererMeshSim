#pragma once
#include <string>
#include "GL\gl3w.h"
#define TEX_TYPE_NORMALS 1
#define TEX_TYPE_DIFFUSE 2
namespace Types {
    class Texture {
        public:
            Texture(void);
            Texture(std::string sFileName, int textureWidth, int textureHeight, int textureBitsPerPixel, int oglTexID, int texID);
            ~Texture();
            std::string sFileName;
            unsigned int textureWidth;
            unsigned int textureHeight;
            unsigned int textureBitsPerPixel;
            unsigned int oglTexID;
            unsigned int texID;
            void bind();
            void unbind();
    };
}
