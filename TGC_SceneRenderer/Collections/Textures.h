#pragma once
#define DEFAULT_TEXTURE_ID 0

#include "../types/Texture.h"
#include <fstream>
#include <iostream>
#include <streambuf>
#include <map>
#include <windows.h>

namespace collections {

    class Textures {
        public:
            virtual ~Textures();
            // Creates Unique Static Instance
            static Textures *Instance();
            // Loads a new texture
            bool loadTexture(const std::string &sFilename, const unsigned int texID, types::Texture::TextureType textureType);
            // Loads a new texture with unique texID (slower)
            bool loadTexture(const std::string &sFilename, types::Texture::TextureType textureType);
            bool unloadTexture(const unsigned int &texID);							// frees memory reserved for texID texture
            bool bindTexture(const unsigned int &texID);							// sets texID texture as the current active texture
            void unloadAllTextures();												// frees memory reserved for all the textures
            unsigned int count(void) const;											// Amount of textures loaded
            unsigned int getDefaultTexture() const { return DEFAULT_TEXTURE_ID; }	// Return default texture's texID, white texture
            types::Texture *getTexture(const unsigned &texID);

        protected:
            Textures(const Textures &cpy);
            Textures();
            static Textures *_eInstance;
            // Texture Collection
            std::map<unsigned int, types::Texture *> textures;
    };

}
