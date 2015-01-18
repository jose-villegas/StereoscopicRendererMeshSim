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
            types::Texture *addTexture(const std::string &sFilename, types::Texture::TextureType textureType);
            // unloads texture from gpu memory and reference
            bool unloadTexture(const unsigned int &texID);
            // unloads texture from gpu memory and reference and frees texture
            // class reserved memory
            bool deteleTexture(const unsigned int &texID);
            // sets texID texture as the current active texture
            bool bindTexture(const unsigned int &texID);
            // unloads all textures from gpu memory and references, warning
            // deletes default texture
            void unloadAllTextures();
            // unloads all textures from gpu memory and references and frees
            // texture class reserved memory, warning deletes default texture
            void deleteAllTextures();
            // amount of textures loaded
            unsigned int textureCount(void) const;
            // return default texture's texID, white default texture
            types::Texture *getDefaultTexture()  { return getTexture(core::EngineData::Commoms::DEFAULT_TEXTURE_ID); }
            types::Texture *getTexture(const unsigned &texID) ;
            // loads the default texture, a white 1x1 texture
            void loadDefaultTexture();

        protected:
            TexturesCollection(const TexturesCollection &cpy);
            TexturesCollection();

            bool preventDuplicates;
            static TexturesCollection *instance;
            static int idCounter;
            // Texture Collection
            std::map<unsigned int, types::Texture *> textures;
    };

}
