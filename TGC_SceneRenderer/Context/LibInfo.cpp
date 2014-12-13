#include "LibInfo.h"
#include "Assimp\version.h"


void LibInfo::Write()
{
    // Write To Logger Current FreeImage Version
    Utils::Logger::Write("Using FreeImage v" +
                         FREEIMAGE_MAJOR_VERSION + "." +
                         FREEIMAGE_MINOR_VERSION + "." +
                         FREEIMAGE_RELEASE_SERIAL + " for texture loading",
                         LOG_CONTEXT_SUCCESS);
    // Write Curret Assimp Version
    Utils::Logger::Write("Using Assimp v" +
                         aiGetVersionMajor() + "." +
                         aiGetVersionMinor() + "." +
                         aiGetVersionRevision() + " for asset loading",
                         LOG_CONTEXT_SUCCESS);
    // Write OGL Version Info
    System::String ^glVersionString = gcnew System::String((char *)glGetString(GL_VERSION));
    System::String ^glslVersionString = gcnew System::String((char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    System::String ^oglString = "OpenGL " + glVersionString + "s, GLSL " + glslVersionString + "\n";
    Utils::Logger::Write(oglString, true, LOG_CONTEXT_SUCCESS);
}
