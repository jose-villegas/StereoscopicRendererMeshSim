#include "Textures.h"
#include "..\Utils\Logger.h"
using namespace ECollections;

Textures *Textures::Instance()
{
    if (!_eInstance) {
        _eInstance = new Textures();
        _eInstance->loadTexture("C:/Users/Admin/Documents/visual studio 2012/Projects/TGC_SceneRenderer/TGC_SceneRenderer/Resources/white.png", 0);
    }

    return _eInstance;
}

Textures::Textures()
{
    // call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
    FreeImage_Initialise();
#endif
}

Textures::~Textures()
{
    // call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
    FreeImage_DeInitialise();
#endif
    unloadAllTextures();
}

bool Textures::loadTexture(const char *filename, const unsigned int texID)
{
    //image width and height
    unsigned int width = 0, height = 0, bpp = 0;
    //OpenGL's image ID to map to
    GLuint gl_texID;
    //check the file signature and deduce its format
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);

    //if still unknown, try to guess the file format from the file extension
    if (fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(filename);
    }

    //if still unkown, return failure
    if (fif == FIF_UNKNOWN) {
        std::string sError = "Error loading texture '" + std::string(filename);
        Utils::Logger::Write(gcnew System::String(sError.c_str()), true, System::Drawing::Color::Red);
        return false;
    }

    //pointer to the image, once loaded
    FIBITMAP *dib = 0;

    //check that the plugin has reading capabilities and load the file
    if (FreeImage_FIFSupportsReading(fif)) {
        dib = FreeImage_Load(fif, filename);
    }

    //if the image failed to load, return failure
    if (!dib) {
        std::string sError = "Error loading texture '" + std::string(filename);
        Utils::Logger::Write(gcnew System::String(sError.c_str()), true, System::Drawing::Color::Red);
        return false;
    }

    //retrieve the image data
    BYTE *bits = FreeImage_GetBits(dib);
    //get the image width and height
    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);
    bpp = FreeImage_GetBPP(dib);

    //if this somehow one of these failed (they shouldn't), return failure
    if ((bits == 0) || (width == 0) || (height == 0)) {
        return false;
    }

    int image_format = bpp == 32 ? GL_BGRA : bpp == 24 ? GL_BGR : bpp == 8 ? GL_RG : 0;
    int internal_format = bpp == 32 ? GL_BGRA : bpp == 24 ? GL_RGB : GL_DEPTH_COMPONENT;

    //if this texture ID is in use, unload the current texture
    if (_eTexCollection.find(texID) != _eTexCollection.end()) {
        glDeleteTextures(1, &(_eTexCollection[texID]->oglTexID));
    }

    glGenTextures(1, &gl_texID);													// generate an OpenGL texture ID for this texture
    _eTexCollection[texID] = new Types::Texture(filename, width, height, bpp, gl_texID, texID);
    glBindTexture(GL_TEXTURE_2D, gl_texID);											// bind to the new texture ID
    // store the texture data for OpenGL use
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, bits);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Free FreeImage's copy of the data
    FreeImage_Unload(dib);
    // return success
    std::string sSuccess = "Loaded texture '" + std::string(filename) + " successfully";
    Utils::Logger::Write(gcnew System::String(sSuccess.c_str()), true, System::Drawing::Color::Green);
    return true;
}

bool Textures::loadTexture(const char *filename)
{
    int unique_texID = 1;

    for (std::map<unsigned int, Types::Texture *>::iterator it = _eTexCollection.begin(); it != _eTexCollection.end(); ++it) {
        if (it->second->texID == unique_texID) {
            unique_texID++;
        } else {
            break;
        }
    }

    return loadTexture(filename, unique_texID);
}

bool Textures::unloadTexture(const unsigned int texID)
{
    bool result = true;

    //if this texture ID mapped, unload it's texture, and remove it from the map
    if (_eTexCollection.find(texID) != _eTexCollection.end()) {
        glDeleteTextures(1, &(_eTexCollection[texID]->oglTexID));
        _eTexCollection.erase(texID);
    }
    //otherwise, unload failed
    else {
        result = false;
    }

    return result;
}

bool Textures::bindTexture(const unsigned int texID)
{
    bool result(true);
    // If this texture ID mapped, bind it's texture as current
    std::map<unsigned int, Types::Texture *>::iterator it = _eTexCollection.find(texID);

    if (it != _eTexCollection.end()) {
        it->second->bind();
    } else {
        // Binding Failed
        result = false;
    }

    return result;
}

void Textures::unloadAllTextures()
{
    //start at the begginning of the texture map
    std::map<unsigned int, Types::Texture *>::iterator it = _eTexCollection.begin();

    //Unload the textures untill the end of the texture map is found
    while (it != _eTexCollection.end()) {
        unloadTexture(it->first);
        it++;
    }

    //clear the texture map
    _eTexCollection.clear();
}

unsigned int Textures::count(void)
{
    return this->_eTexCollection.size();
}

Textures *ECollections::Textures::_eInstance = nullptr;
