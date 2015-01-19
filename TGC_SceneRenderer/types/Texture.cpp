#include "Texture.h"
#include <iostream>
using namespace types;

types::Texture::Texture(const std::string &sFilename, const unsigned int &texId, const TextureType &tType)
{
    this->sFilename        = sFilename;
    this->texId            = texId;
    this->textureType      = tType;
    this->bitsPerPixel     = 0;
    this->width            = 0;
    this->height           = 0;
    this->oglTexId         = 0;
    this->referenceCount   = 0;
    this->generateMipmaps  = true;
    this->enableAnisotropic = true;
    this->textureDimension = GL_TEXTURE_2D;
    this->internalFormat = GL_RGB;
    this->readType = GL_UNSIGNED_BYTE;
    this->minFilteringMode = magFilteringMode = LinearMipmapLinear;
    this->sWrappingMode = tWrappingMode = Repeat;
}

types::Texture::Texture(const unsigned int &texId, const TextureType &tType)
{
    this->sFilename        = "Texture";
    this->texId            = texId;
    this->textureType      = tType;
    this->bitsPerPixel     = 0;
    this->width            = 0;
    this->height           = 0;
    this->referenceCount   = 0;
    this->oglTexId         = 0;
    this->generateMipmaps  = true;
    this->enableAnisotropic = true;
    this->textureDimension = GL_TEXTURE_2D;
    this->internalFormat = GL_RGB;
    this->readType = GL_UNSIGNED_BYTE;
    this->minFilteringMode = magFilteringMode = LinearMipmapLinear;
    this->sWrappingMode = tWrappingMode = Repeat;
}

types::Texture::Texture()
{
    this->sFilename        = "Empty";
    this->texId            = 0;
    this->textureType      = None;
    this->bitsPerPixel     = 0;
    this->width            = 0;
    this->height           = 0;
    this->referenceCount   = 0;
    this->oglTexId         = -1;
    this->generateMipmaps  = true;
    this->enableAnisotropic = true;
    this->textureDimension = GL_TEXTURE_2D;
    this->minFilteringMode = magFilteringMode = LinearMipmapLinear;
}

bool types::Texture::loadTexture()
{
    return loadTexture(this->sFilename);
}

bool types::Texture::loadTexture(const std::string &sFilename)
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
    format = bitsPerPixel == 32 ? GL_BGRA :
             bitsPerPixel == 24 ? GL_BGR  :
             bitsPerPixel == 16 ? GL_RG   :
             bitsPerPixel ==  8 ? GL_RED  : 0;
    glGenTextures(1, &oglTexId);
    glBindTexture(GL_TEXTURE_2D, oglTexId);											// bind to the new texture ID
    // store the texture data for OpenGL use
    internalFormat = GL_RGBA8;
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, readType, bits);
    // set texture filtering mode
    this->setFilteringMode(this->minFilteringMode, this->magFilteringMode, this->generateMipmaps);
    // set wrapping mode
    this->setWrappingMode(this->sWrappingMode, this->tWrappingMode);

    // anisotropic ( better quality texture filtering if available )
    if (evaluateAnisoLevel(this, anisotropicFilteringLevel)) {
        glTexParameterf(GL_TEXTURE_2D, TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)anisotropicFilteringLevel);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    // Free FreeImage's copy of the data
    FreeImage_Unload(dib);
    this->sFilename = sFilename;
    // successful texture load
    return true;
}

void types::Texture::unload()
{
    if (oglTexId == 0) { return; }

    glDeleteTextures(1, &oglTexId);
    // set texture as invalid
    this->oglTexId = 0;
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

        case Opacity   : return "Ocapacity"; break;

        case Displacement: return "Displacement"; break;

        case Lightmap    : return "Lightmap"; break;

        case Reflection  : return "Reflection"; break;
    }

    return "";
}

void types::Texture::setFilteringMode(const TextureFilteringMode min, const TextureFilteringMode mag, const bool generateMipmaps)
{
    this->minFilteringMode = min;
    this->magFilteringMode = mag;
    this->generateMipmaps = generateMipmaps;

    if (oglTexId == 0) { return; }

    glBindTexture(GL_TEXTURE_2D, oglTexId);

    // filtering mode
    if (this->generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
        // bilinear or trilinear
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)this->minFilteringMode);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)this->magFilteringMode);
    } else {
        // bilinear o discrete
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)std::min(this->minFilteringMode, Linear));
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)std::min(this->magFilteringMode, Linear));
    }
}

void types::Texture::setWrappingMode(const TextureWrappingMode wrapS, const TextureWrappingMode wrapT)
{
    this->sWrappingMode = wrapS;
    this->tWrappingMode = wrapT;

    if (oglTexId == 0) { return; }

    // bind and set wrapping mode
    glBindTexture(GL_TEXTURE_2D, oglTexId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapT);
}

bool types::Texture::evaluateAnisoLevel(Texture *tex, const float level)
{
    return tex->enableAnisotropic && core::EngineData::AnisotropicFilteringAvaible() && level > 0 && level <= core::EngineData::MaxAnisotropicFilteringAvaible();
}

void types::Texture::setAnisotropicFilteringLevel(const float level)
{
    if (core::EngineData::AnisotropicFilteringAvaible() && level > 0 && level <= core::EngineData::MaxAnisotropicFilteringAvaible()) {
        anisotropicFilteringLevel = level;
    } else { anisotropicFilteringLevel = 0; }
}

bool types::Texture::enableAnisotropicFiltering(bool val)
{
    this->enableAnisotropic = val;

    if (oglTexId == 0) { return false; }

    if (evaluateAnisoLevel(this, this->anisotropicFilteringLevel)) {
        glBindTexture(GL_TEXTURE_2D, oglTexId);
        glTexParameterf(GL_TEXTURE_2D, TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)anisotropicFilteringLevel);
        return this->enableAnisotropic = val;
    } else {
        glBindTexture(GL_TEXTURE_2D, oglTexId);
        glTexParameterf(GL_TEXTURE_2D, TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)0.f);
    }

    return this->enableAnisotropic = false;
}

void types::Texture::createTexture(void *rawData)
{
    // The texture we're going to render to
    glGenTextures(1, &oglTexId);
    // bind the new texture
    glBindTexture(GL_TEXTURE_2D, oglTexId);
    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->format, this->readType, rawData);
    // set filtering mode
    this->setFilteringMode(this->minFilteringMode, this->magFilteringMode, this->generateMipmaps);
    // set wrapping mode
    this->setWrappingMode(this->sWrappingMode, this->tWrappingMode);
}

void types::Texture::createTexture()
{
    // The texture we're going to render to
    glGenTextures(1, &oglTexId);
    // bind the new texture
    glBindTexture(GL_TEXTURE_2D, oglTexId);
    // create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->format, this->readType, 0);
    // set filtering mode
    this->setFilteringMode(this->minFilteringMode, this->magFilteringMode, this->generateMipmaps);
    // set wrapping mode
    this->setWrappingMode(this->sWrappingMode, this->tWrappingMode);
}

void types::Texture::createTexture(const unsigned int width,
                                   const unsigned int height,
                                   const TextureFilteringMode min /*= Nearest*/,
                                   const TextureFilteringMode mag /*= Nearest*/,
                                   const TextureWrappingMode sWrap /*= Repeat*/,
                                   const TextureWrappingMode tWrap /*= Repeat*/,
                                   const bool generateMipmaps /*= false*/,
                                   const unsigned int readType /* = GL_UNSIGNED_BYTE*/,
                                   const GLint internalFormat /*= GL_RGB*/,
                                   const GLint format /*= GL_RGB*/,
                                   void *rawData /*= nullptr*/)
{
    this->width = width; this->height = height;
    this->minFilteringMode = min;
    this->magFilteringMode = mag;
    this->sWrappingMode = sWrap;
    this->tWrappingMode = tWrap;
    this->generateMipmaps = generateMipmaps;
    this->internalFormat = internalFormat;
    this->format = format;
    this->readType = readType;
    // create textue with passed params
    rawData == nullptr ? this->createTexture() : this->createTexture(rawData);
}

void types::Texture::unbind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    if (oglTexId == 0) { return; }
}

void Texture::bind() const
{
    glActiveTexture(GL_TEXTURE0 + (int)this->textureType);
    glBindTexture(GL_TEXTURE_2D, oglTexId);
}

float types::Texture::anisotropicFilteringLevel = 0;
