#include "Texture.h"
#include <iostream>
using namespace types;

types::Texture::Texture(const std::string &sFilename, const unsigned int &texId, const TextureType &tType)
{
    this->sFilename     = sFilename;
    this->texId         = texId;
    this->textureType   = tType;
    this->bitsPerPixel  = 0;
    this->width         = 0;
    this->height        = 0;
    this->oglTexId      = -1;
}

types::Texture::Texture(const unsigned int &texId, const TextureType &tType)
{
    this->sFilename     = "Texture";
    this->texId         = texId;
    this->textureType   = tType;
    this->bitsPerPixel  = 0;
    this->width         = 0;
    this->height        = 0;
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
    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);
    bitsPerPixel = FreeImage_GetBPP(dib);

    // If this somehow one of these failed (they shouldn't), return failure
    if ((bitsPerPixel == 0) || (height == 0) || (width == 0)) {
        FreeImage_Unload(dib);
        return false;
    }

    // Check Image Bit Density
    GLuint imageFormat = bitsPerPixel == 32 ? GL_BGRA :
                         bitsPerPixel == 24 ? GL_BGR  :
                         bitsPerPixel == 16 ? GL_RG   :
                         bitsPerPixel ==  8 ? GL_RED  : 0;
    glGenTextures(1, &oglTexId);
    glBindTexture(GL_TEXTURE_2D, oglTexId);											// bind to the new texture ID
    // store the texture data for OpenGL use
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, bits);
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

std::string types::Texture::getTextureTypeString()
{
    switch (textureType) {
        case None        : return "None"; break;

        case Diffuse     : return "Diffuse"; break;

        case Specular    : return "Specular"; break;

        case Ambient     : return "Ambient"; break;

        case Emissive    : return "Emissive"; break;

        case Height      : return "Height"; break;

        case Normals     : return "Normals"; break;

        case Shininess   : return "Shininess"; break;

        case Ocapacity   : return "Ocapacity"; break;

        case Displacement: return "Displacement"; break;

        case Lightmap    : return "Lightmap"; break;

        case Reflection  : return "Reflection"; break;
    }

    return "";
}

Texture::~Texture()
{
}

void Texture::bind() const
{
    glActiveTexture(texId);
    glBindTexture(GL_TEXTURE_2D, oglTexId);
}
