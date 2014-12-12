#pragma once
#include <string>
#include "GL\gl_core_4_4.h"

enum TEXTURE_TYPE {
    DIFFUSE,
    NORMALS,
    REFLECTION,
    LIGHTMAP
};

namespace Types {
    ref class Texture {
        public:
            Texture(void);
            Texture(System::String ^sFileName, int textureWidth, int textureHeight, int textureBitsPerPixel, int oglTexID, int texID);
            ~Texture();
            System::String ^sFileName;
            unsigned int tWidth;
            unsigned int tHeight;
            unsigned int tBitsPerPixel;
            unsigned int oglTexID;
            unsigned int texID;
            void bind();
    };
}
