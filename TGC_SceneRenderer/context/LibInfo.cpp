#include "LibInfo.h"

void LibInfo::Write()
{
    // Write Current FreeImage Version
    FREEIMAGE_INFO_STRING_STD = new std::string(
        "Using FreeImage v"
        + std::to_string(FREEIMAGE_MAJOR_VERSION)
        + "." + std::to_string(FREEIMAGE_MINOR_VERSION)
        + "." + std::to_string(FREEIMAGE_RELEASE_SERIAL)
        + " for texture loading"
    );
    FREEIMAGE_INFO_STRING = gcnew System::String(FREEIMAGE_INFO_STRING_STD->c_str());
    // Write Curret Assimp
    ASSIMP_INFO_STRING_STD = new std::string(
        "Using Assimp v"
        + std::to_string(aiGetVersionMajor())
        + "." + std::to_string(aiGetVersionMinor())
        + "." + std::to_string(aiGetVersionRevision())
        + " for asset loading"
    );
    ASSIMP_INFO_STRING = gcnew System::String(ASSIMP_INFO_STRING_STD->c_str());;
    // Write OGL Version Info
    OGL_INFO_STRING_STD = new std::string(
        "OpenGL "
        + std::string((char *)glGetString(GL_VERSION))
        + "s, GLSL "
        + std::string((char *)glGetString(GL_SHADING_LANGUAGE_VERSION))
    );
    OGL_INFO_STRING = gcnew System::String(OGL_INFO_STRING_STD->c_str());
    // Output General Info
    std::cout << *FREEIMAGE_INFO_STRING_STD << std::endl;
    std::cout << *ASSIMP_INFO_STRING_STD << std::endl;
    std::cout << *OGL_INFO_STRING_STD << std::endl;
    std::cout << "**************************************************************************\n" << std::endl;
}
