#pragma once

#include "../Types/Texture.h"
#include "GL/gl_core_4_4.h"
#include "FreeImage/FreeImage.h"
#include <map>
#define DEFAULT_TEXTURE_ID 0
namespace ECollections {

    class Textures {
        public:
            static Textures *Instance();
            virtual ~Textures();
            // Loads a Texture if texID is already taken it will be replaced with the new texture
            // filename: where to load the file from
            // texID: arbitrary id you will reference the texture by if texID is taken previous texture will be taken out
            // image_format: format the image is in (default GL_RGB)
            // internal_format: format to store the image in (default GL_RGB)
            // level: mipmapping level (default 0)
            // border: border size (default 0)
            bool loadTexture(const char *filename,
                             const unsigned int texID,
                             GLenum image_format = GL_RGB,
                             GLint internal_format = GL_RGB,
                             GLint level = 0,
                             GLint border = 0);
            // Loads a new texture with unique texID
            // image_format: format the image is in (default GL_RGB)
            // internal_format: format to store the image in (default GL_RGB)
            // level: mipmapping level (default 0)
            // border: border size (default 0)
            bool loadTexture(const char *filename,
                             GLenum image_format = GL_RGB,
                             GLint internal_format = GL_RGB,
                             GLint level = 0,
                             GLint border = 0);
            bool unloadTexture(const unsigned int texID);		// free the memory for a texture
            bool bindTexture(const unsigned int texID);			// set the current texture
            void unloadAllTextures();							// free all texture memory
            unsigned int count(void);
            unsigned int getDefaultTexture() { return DEFAULT_TEXTURE_ID; }

        protected:
            Textures();
            Textures(const Textures &tm);
            Textures &operator=(const Textures &tm);
            static Textures *_eInstance;
            std::map<unsigned int, Types::Texture *> m_texID;
    };

}
