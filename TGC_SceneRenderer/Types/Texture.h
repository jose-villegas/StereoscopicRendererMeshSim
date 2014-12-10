#pragma once
#include <string>
#include "GL\gl3w.h"
#define TEX_TYPE_NORMALMAP 1
#define TEX_TYPE_COLORMAP 2

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

