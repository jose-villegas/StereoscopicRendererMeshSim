#include "Texture.h"
using namespace Types;

Texture::Texture(void)
{
}

Texture::Texture(std::string, int tw, int textureHeight, int textureBitsPerPixel, int oglTexID, int texID)
{
    this->sFileName = sFileName;
    this->tWidth = tw;
    this->tHeight = textureHeight;
    this->tBitsPerPixel = textureBitsPerPixel;
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
