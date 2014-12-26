#pragma once

#include "../types/Texture.h"
#include "../core/Data.h"
#include <fstream>
#include <iostream>
#include <streambuf>
#include <map>
#include <windows.h>

namespace collections {

    class TexturesCollection {
        public:
            virtual ~TexturesCollection();
            // creates Unique Static Instance
            static TexturesCollection *Instance();
            // loads a new texture
            bool addTexture(const std::string &sFilename, const unsigned int texID, types::Texture::TextureType textureType);
            // loads a new texture with unique texID (slower)
            bool addTexture(const std::string &sFilename, types::Texture::TextureType textureType);
            // frees memory reserved for texID texture
            bool unloadTexture(const unsigned int &texID);
            // sets texID texture as the current active texture
            bool bindTexture(const unsigned int &texID);
            // frees memory reserved for all the textures
            void unloadAllTextures();
            // amount of textures loaded
            unsigned int textureCount(void) const;
            // return default texture's texID, white default texture
            types::Texture *getDefaultTexture() { return getTexture(core::EngineData::Commoms::DEFAULT_TEXTURE_ID); }
            types::Texture *getTexture(const unsigned &texID);

        protected:
            TexturesCollection(const TexturesCollection &cpy);
            TexturesCollection();
            static TexturesCollection *instance;
            // Texture Collection
            std::map<unsigned int, types::Texture *> textures;
    };

}
