#include "Texture.h"

Types::Texture::Texture(void)
{
}

Types::Texture::Texture(std::string sFileName, int textureWidth, int textureHeight, int textureBitsPerPixel, int oglTexID, int texID)
{
    this->sFileName = sFileName;
    this->textureWidth = textureWidth;
    this->textureHeight = textureHeight;
    this->textureBitsPerPixel = textureBitsPerPixel;
    this->oglTexID = oglTexID;
    this->texID = texID;
}

Types::Texture::~Texture()
{
}

void Types::Texture::bind()
{
    glActiveTexture(texID);
    glBindTexture(GL_TEXTURE_2D, oglTexID);
}
