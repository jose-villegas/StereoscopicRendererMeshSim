#include "ResourcesLoader.h"
using namespace core;


void core::ResourcesLoader::loadDefaultTexture(collections::Textures *_texCollection)
{
    // Load Default Resources
    _texCollection->loadTexture("../TGC_SceneRenderer/resources/default.png", 0, types::Texture::Diffuse);
}
