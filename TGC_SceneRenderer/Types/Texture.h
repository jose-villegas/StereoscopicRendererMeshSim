#pragma once
#include <string>
#include "GL\gl_core_4_4.h"
#include "FreeImage\FreeImage.h"
#include "..\Utils\Logger.h"
enum TEXTURE_TYPE {
    DIFFUSE,
    NORMALS,
    REFLECTION,
    LIGHTMAP
};

namespace Types {
    class Texture {
        public:
            Texture(void);
            Texture(std::string sFilename, const unsigned int texID);
            ~Texture();
            std::string sFilename;
            unsigned int tWidth;
            unsigned int tHeight;
            unsigned int tBitsPerPixel;
            unsigned int oglTexID;
            unsigned int texID;
            unsigned int internalFormat;
            unsigned int imageFormat;
            void bind();
            bool load(const char *sFilename);
            void unload();
    };
}
