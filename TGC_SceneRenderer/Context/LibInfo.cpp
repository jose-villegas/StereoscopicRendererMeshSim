#include "LibInfo.h"
#include "Assimp\version.h"

void LibInfo::Write()
{
    // Write To Logger Current FreeImage Version
    FREEIMAGE_INFO_STRING = "Using FreeImage v" + FREEIMAGE_MAJOR_VERSION + "." + FREEIMAGE_MINOR_VERSION + "." + FREEIMAGE_RELEASE_SERIAL +
                            " for texture loading";
    Utils::Logger::Write(FREEIMAGE_INFO_STRING, LOG_CONTEXT_SUCCESS);
    // Write Curret Assimp Version
    ASSIMP_INFO_STRING = "Using Assimp v" + aiGetVersionMajor() + "." + aiGetVersionMinor() + "." + aiGetVersionRevision() +
                         " for asset loading";
    Utils::Logger::Write(ASSIMP_INFO_STRING, LOG_CONTEXT_SUCCESS);
    // Write OGL Version Info
    OGL_INFO_STRING = "OpenGL " + gcnew System::String((char *)glGetString(GL_VERSION)) + "s, GLSL " +
                      gcnew System::String((char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    Utils::Logger::Write(OGL_INFO_STRING, true, LOG_CONTEXT_SUCCESS);
}