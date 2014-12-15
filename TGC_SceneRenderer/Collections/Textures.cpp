#include "Textures.h"
#include "..\Utils\Logger.h"
#include "..\Types\Texture.h"
#include <fstream>
#include <iostream>
#include <streambuf>
using namespace ECollections;

Textures *Textures::Instance()
{
    if (!_eInstance) {
        // Create Unique Shared Static Instance
        _eInstance = new Textures();
        // Load Default Resources
        _eInstance->loadTexture("../TGC_SceneRenderer/Resources/default.png", 0, Types::Texture::Diffuse);
    }

    return _eInstance;
}

Textures::Textures()
{
}

Textures::~Textures()
{
    unloadAllTextures();
}

bool Textures::loadTexture(const std::string &sFilename, const unsigned int texID, Types::Texture::TextureType textureType)
{
    Types::Texture *newTex = new Types::Texture(sFilename, texID, textureType);
    bool loadingResult = newTex->load(sFilename);

    if (!loadingResult) {
        Utils::Logger::Write("Error loading " + std::string(sFilename) + " texture", true, LOG_CONTEXT_DANGER);
        return loadingResult;
    }

    //if this texture ID is in use, unload the current texture
    std::map<unsigned int, Types::Texture *>::iterator it = _eTexCollection.find(texID);

    if (it != _eTexCollection.end()) {
        Utils::Logger::Write("Warning texture: " + _eTexCollection[texID]->sFilename + " replaced", true, LOG_CONTEXT_WARNING);
        glDeleteTextures(1, &(it->second->oglTexID));
    }

    _eTexCollection[texID] = newTex;
    Utils::Logger::Write("Texture: " + std::string(sFilename) + "loaded successfully", true, LOG_CONTEXT_SUCCESS);
    return loadingResult;
}

bool Textures::loadTexture(const std::string &sFilename, Types::Texture::TextureType textureType)
{
    int unique_texID = 1;

    for (std::map<unsigned int, Types::Texture *>::iterator it = _eTexCollection.begin(); it != _eTexCollection.end(); ++it) {
        if (it->second->texID == unique_texID) {
            unique_texID++;
        } else {
            break;
        }
    }

    return loadTexture(sFilename, unique_texID, textureType);
}

bool Textures::unloadTexture(const unsigned int texID)
{
    bool result = true;
    //if this texture ID mapped, unload it's texture, and remove it from the map
    std::map<unsigned int, Types::Texture *>::iterator it = _eTexCollection.find(texID);

    if (it != _eTexCollection.end()) {
        it->second->unload();
        _eTexCollection.erase(texID);
    }
    //otherwise, unload failed
    else {
        result = false;
    }

    return result;
}

bool Textures::bindTexture(const unsigned int texID)
{
    bool result(true);
    // If this texture ID mapped, bind it's texture as current
    std::map<unsigned int, Types::Texture *>::iterator it = _eTexCollection.find(texID);

    if (it != _eTexCollection.end()) {
        it->second->bind();
    } else {
        // Binding Failed
        result = false;
    }

    return result;
}

void Textures::unloadAllTextures()
{
    //start at the begginning of the texture map
    std::map<unsigned int, Types::Texture *>::iterator it = _eTexCollection.begin();

    //Unload the textures untill the end of the texture map is found
    while (it != _eTexCollection.end()) {
        unloadTexture(it->first);
        it++;
    }

    //clear the texture map
    _eTexCollection.clear();
}

unsigned int Textures::count(void)
{
    return this->_eTexCollection.size();
}

Textures *ECollections::Textures::_eInstance = nullptr;