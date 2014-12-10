#include "Texture.h"

Texture::Texture(void)
{
}

Texture::Texture(std::string sFileName, int textureWidth, int textureHeight, int textureBitsPerPixel, int oglTexID, int texID)
{
    this->sFileName = sFileName;
    this->textureWidth = textureWidth;
    this->textureHeight = textureHeight;
    this->textureBitsPerPixel = textureBitsPerPixel;
    this->oglTexID = oglTexID;
    this->texID = texID;
}

Texture::~Texture()
{
}

void Texture::bind()
{
    glActiveTexture(texID);
    glBindTexture(GL_TEXTURE_2D, oglTexID);
}
