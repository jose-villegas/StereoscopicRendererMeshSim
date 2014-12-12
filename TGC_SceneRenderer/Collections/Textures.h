#pragma once

#include "../Types/Texture.h"
#include "GL/gl_core_4_4.h"
#include "FreeImage/FreeImage.h"
#include <map>
#define DEFAULT_TEXTURE_ID 0
namespace ECollections {

    ref class Textures {
        public:
            // Creates Unique Static Instance
            static Textures ^Instance();
            virtual ~Textures();
            // Loads a Texture if texID is already taken it will be replaced with the new texture
            // filename: where to load the file from
            // texID: arbitrary id you will reference the texture by if texID is taken previous texture will be taken out
            // image_format: format the image is in (usually GL_RGB)
            // internal_format: format to store the image in (usually GL_RGB)
            // level: mipmapping level (usually 0)
            // border: border size (usually 0)
            bool loadTexture(const char *filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border);
            // Loads a new texture with unique texID
            // image_format: format the image is in (usually GL_RGB)
            // internal_format: format to store the image in (usually GL_RGB)
            // level: mipmapping level (usually 0)
            // border: border size (usually 0)
            bool loadTexture(const char *filename, GLenum image_format, GLint internal_format, GLint level, GLint border);
            bool unloadTexture(const unsigned int texID);					// frees memory reserved for texID texture
            bool bindTexture(const unsigned int texID);						// sets texID texture as the current active texture
            void unloadAllTextures();										// frees memory reserved for all the textures
            unsigned int count(void);										// Amount of textures loaded
            unsigned int getDefaultTexture() { return DEFAULT_TEXTURE_ID; }	// Return default texture's texID, white texture

        protected:
            Textures();
            static Textures ^_eInstance = nullptr;
            // Texture Collection
            System::Collections::Hashtable ^_eTexCollection;
    };

}
