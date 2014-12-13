#include "Texture.h"


using namespace Types;

Texture::Texture(void)
{
}

Texture::Texture(std::string sFilename, int textureWidth, int textureHeight, int textureBitsPerPixel, int oglTexID, int texID)
{
    this->sFilename = sFilename;
    this->tWidth = textureWidth;
    this->tHeight = textureHeight;
    this->tBitsPerPixel = textureBitsPerPixel;
    this->oglTexID = oglTexID;
    this->texID = texID;
}

Types::Texture::Texture(std::string sFilename, const unsigned int texID)
{
    this->sFilename = sFilename;
    this->texID = texID;
}

bool Types::Texture::load(const char *sFilename)
{
    //check the file signature and deduce its format
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(sFilename, 0);

    //if still unknown, try to guess the file format from the file extension
    if (fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(sFilename);
    }

    //if still unkown, return failure
    if (fif == FIF_UNKNOWN) {
        return false;
    }

    //pointer to the image, once loaded
    FIBITMAP *dib = 0;

    //check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif)) {
        dib = FreeImage_Load(fif, sFilename);
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
    image_format = tBitsPerPixel == 32 ? GL_BGRA : tBitsPerPixel == 24 ? GL_BGR : tBitsPerPixel == 8 ? GL_RG : 0;
    internal_format = tBitsPerPixel == 32 ? GL_BGRA : tBitsPerPixel == 24 ? GL_RGB : GL_DEPTH_COMPONENT;
    glGenTextures(1, &oglTexID);
    glBindTexture(GL_TEXTURE_2D, oglTexID);											// bind to the new texture ID
    // store the texture data for OpenGL use
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, tWidth, tHeight, 0, image_format, GL_UNSIGNED_BYTE, bits);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Free FreeImage's copy of the data
    FreeImage_Unload(dib);
    return true;
}

Texture::~Texture()
{
}

void Texture::bind()
{
    glActiveTexture(texID);
    glBindTexture(GL_TEXTURE_2D, oglTexID);
}
