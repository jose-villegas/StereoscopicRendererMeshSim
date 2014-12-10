#pragma once
#include <windows.h>
#include <GL/gl3w.h>
#include "FreeImage/FreeImage.h"
#include <map>

class TextureCollection {
        static TextureCollection *Inst();
        virtual ~TextureCollection();
        // load a texture an make it the current texture
        // if texID is already in use, it will be unloaded and replaced with this texture
        bool loadTexture(const char *filename,				//where to load the file from
                         const unsigned int texID,			//arbitrary id you will reference the texture by
                         GLenum image_format = GL_RGB,		//format the image is in
                         GLint internal_format = GL_RGB,	//format to store the image in
                         GLint level = 0,					//mipmapping level
                         GLint border = 0);					//border size
        bool unloadTexture(const unsigned int texID);	//free the memory for a texture
        bool bindTexture(const unsigned int texID);		//set the current texture
        void unloadAllTextures();						//free all texture memory

    protected:
        TextureCollection();
        TextureCollection(const TextureCollection &tm);
        TextureCollection &operator=(const TextureCollection &tm);
        static TextureCollection *m_inst;
        std::map<unsigned int, GLuint> m_texID;
};

