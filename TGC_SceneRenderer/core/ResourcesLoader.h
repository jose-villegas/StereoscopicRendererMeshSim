#pragma once
#include "../collections/stored/Shaders.h"
#include "../collections/TexturesCollection.h"
namespace core {

    class ResourcesLoader {
        public:
            static void loadDefaultTexture(collections::TexturesCollection *_texCollection);
            static void loadStoredShaders();
    };
}

