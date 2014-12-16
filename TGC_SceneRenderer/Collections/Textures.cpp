#include "Textures.h"
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
    bool loadingResult = newTex->load();

    if (!loadingResult) {
        std::cout << "Textures(" << this << "): " << "Error loading " << sFilename << " texture" << std::endl;
        return loadingResult;
    }

    //if this texture ID is in use, unload the current texture
    std::map<unsigned int, Types::Texture *>::iterator it = textures.find(texID);

    if (it != textures.end()) {
        std::cout << "Textures(" << this << "): " << "Warning texture: " << textures[texID]->getFilename() << " replaced" << std::endl;
        it->second->unload();
    }

    // Store new texID and return success
    textures[texID] = newTex;
    std::cout << "Textures(" << this << "): " << "Texture " << std::string(sFilename) << " loaded successfully" << std::endl;
    return loadingResult;
}

bool Textures::loadTexture(const std::string &sFilename, Types::Texture::TextureType textureType)
{
    int unique_texID = 1;

    for (std::map<unsigned int, Types::Texture *>::iterator it = textures.begin(); it != textures.end(); ++it) {
        if (it->second->geTexID() == unique_texID) {
            unique_texID++;
        } else {
            break;
        }
    }

    return loadTexture(sFilename, unique_texID, textureType);
}

bool Textures::unloadTexture(const unsigned int &texID)
{
    bool result = true;
    // if this texture ID mapped, unload it's texture, and remove it from the map
    std::map<unsigned int, Types::Texture *>::iterator it = textures.find(texID);

    if (it != textures.end()) {
        it->second->unload();
        textures.erase(texID);
    } else {
        result = false;
    }

    return result;
}

bool Textures::bindTexture(const unsigned int &texID)
{
    bool result(true);
    // If this texture ID mapped, bind it's texture as current
    std::map<unsigned int, Types::Texture *>::iterator it = textures.find(texID);

    if (it != textures.end()) {
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
    std::map<unsigned int, Types::Texture *>::iterator it = textures.begin();

    // Collection already empty
    if (it == textures.end()) { return; }

    //Unload the textures untill the end of the texture map is found
    do { it->second->unload(); } while (++it != textures.end());

    //clear the texture map
    textures.clear();
}

unsigned int Textures::count(void) const
{
    return this->textures.size();
}

Types::Texture *ECollections::Textures::getTexture(const unsigned &texID)
{
    return this->textures[texID];
}

Textures *ECollections::Textures::_eInstance = nullptr;
