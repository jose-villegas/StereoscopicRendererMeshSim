#include "Texture.h"
#include <iostream>
using namespace types;

types::Texture::Texture(const std::string &sFilename, const unsigned int &texId, const TextureType &tType)
{
    this->sFilename     = sFilename;
    this->texId         = texId;
    this->tType         = tType;
    this->tBitsPerPixel = 0;
    this->tWidth        = 0;
    this->tHeight       = 0;
    this->oglTexId      = -1;
}

types::Texture::Texture(const unsigned int &texId, const TextureType &tType)
{
    this->sFilename     = "Texture";
    this->texId         = texId;
    this->tType         = tType;
    this->tBitsPerPixel = 0;
    this->tWidth        = 0;
    this->tHeight       = 0;
    this->oglTexId      = -1;
}

bool types::Texture::load()
{
    return load(this->sFilename);
}

bool types::Texture::load(const std::string &sFilename)
{
    //check the file signature and deduce its format
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(sFilename.c_str(), 0);

    //if still unknown, try to guess the file format from the file extension
    if (fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(sFilename.c_str());
    }

    //if still unkown, return failure
    if (fif == FIF_UNKNOWN) {
        return false;
    }

    //pointer to the image, once loaded
    FIBITMAP *dib = 0;

    //check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif)) {
        dib = FreeImage_Load(fif, sFilename.c_str());
    }

    //if the image failed to load, return failure
    if (!dib) {
        return false;
    }

    // Convert to 32 for unified texture BPP
    FreeImage_ConvertTo32Bits(dib);
    // Retrieve the image raw data
    BYTE *bits = FreeImage_GetBits(dib);
    //get the image width and height
    tWidth = FreeImage_GetWidth(dib);
    tHeight = FreeImage_GetHeight(dib);
    tBitsPerPixel = FreeImage_GetBPP(dib);

    // If this somehow one of these failed (they shouldn't), return failure
    if ((tBitsPerPixel == 0) || (tHeight == 0) || (tWidth == 0)) {
        FreeImage_Unload(dib);
        return false;
    }

    // Check Image Bit Density
    GLuint imageFormat = tBitsPerPixel == 32 ? GL_BGRA :
                         tBitsPerPixel == 24 ? GL_BGR  :
                         tBitsPerPixel == 16 ? GL_RG   :
                         tBitsPerPixel ==  8 ? GL_RED  : 0;
    glGenTextures(1, &oglTexId);
    glBindTexture(GL_TEXTURE_2D, oglTexId);											// bind to the new texture ID
    // store the texture data for OpenGL use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tWidth, tHeight, 0, imageFormat, GL_UNSIGNED_BYTE, bits);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Free FreeImage's copy of the data
    FreeImage_Unload(dib);
    this->sFilename = sFilename;
    return true;
}

void types::Texture::unload() const
{
    glDeleteTextures(1, &oglTexId);
}

Texture::~Texture()
{
}

void Texture::bind() const
{
    glActiveTexture(texId);
    glBindTexture(GL_TEXTURE_2D, oglTexId);
}
