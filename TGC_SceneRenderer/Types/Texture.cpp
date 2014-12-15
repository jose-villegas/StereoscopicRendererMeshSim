#include "Texture.h"
using namespace Types;

Texture::Texture(void)
{
}

Types::Texture::Texture(std::string sFilename, const unsigned int texID, TextureType tType)
{
    this->sFilename = sFilename;
    this->texID = texID;
    this->tType = tType;
}

bool Types::Texture::load(std::string sFilename)
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

    //retrieve the image data
    BYTE *bits = FreeImage_GetBits(dib);
    //get the image width and height
    tWidth = FreeImage_GetWidth(dib);
    tHeight = FreeImage_GetHeight(dib);
    tBitsPerPixel = FreeImage_GetBPP(dib);

    //if this somehow one of these failed (they shouldn't), return failure
    if ((tBitsPerPixel == 0) || (tHeight == 0) || (tWidth == 0)) {
        FreeImage_Unload(dib);
        return false;
    }

    // Check Image Bit Density
    imageFormat = tBitsPerPixel == 32 ? GL_BGRA : tBitsPerPixel == 24 ? GL_BGR : tBitsPerPixel == 8 ? GL_RG : 0;
    internalFormat = tBitsPerPixel == 32 ? GL_BGRA : tBitsPerPixel == 24 ? GL_RGB : GL_DEPTH_COMPONENT;
    glGenTextures(1, &oglTexID);
    glBindTexture(GL_TEXTURE_2D, oglTexID);											// bind to the new texture ID
    // store the texture data for OpenGL use
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tWidth, tHeight, 0, imageFormat, GL_UNSIGNED_BYTE, bits);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Free FreeImage's copy of the data
    FreeImage_Unload(dib);
    return true;
}

void Types::Texture::unload() const
{
    glDeleteTextures(1, &oglTexID);
}

Texture::~Texture()
{
}

void Texture::bind() const
{
    glActiveTexture(texID);
    glBindTexture(GL_TEXTURE_2D, oglTexID);
}
