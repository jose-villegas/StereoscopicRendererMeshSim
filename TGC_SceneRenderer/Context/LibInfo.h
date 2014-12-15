#pragma once
// #define FREEIMAGE_LIB
#include "..\Collections\Textures.h"
#include "FreeImage/FreeImage.h"
#include "..\Utils\Logger.h"
#include "Assimp\defs.h"
#include "Assimp\version.h"

ref class LibInfo {
    public:
        static ECollections::Textures *_tInstance;
        static System::String ^OGL_INFO_STRING;
        static System::String ^FREEIMAGE_INFO_STRING;
        static System::String ^ASSIMP_INFO_STRING;
        static void Write();
};
