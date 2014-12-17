#pragma once
#include "../collections/stored/Shaders.h"
#include "../collections/Textures.h"
namespace core {

    class ResourcesLoader {
        public:
            static void loadDefaultTexture(collections::Textures *_texCollection);
            static void loadStoredShaders();
    };
}

