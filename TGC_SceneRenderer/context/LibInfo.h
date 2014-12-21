#pragma once
// #define FREEIMAGE_LIB
#include "../collections/TexturesCollection.h"
#include "FreeImage/FreeImage.h"
#include "Assimp/defs.h"
#include "Assimp/version.h"
#include <iostream>

ref class LibInfo {
    public:
        static collections::TexturesCollection *_tInstance;
        static System::String ^OGL_INFO_STRING;
        static System::String ^FREEIMAGE_INFO_STRING;
        static System::String ^ASSIMP_INFO_STRING;
        static std::string *OGL_INFO_STRING_STD;
        static std::string *FREEIMAGE_INFO_STRING_STD;
        static std::string *ASSIMP_INFO_STRING_STD;
        static void Write();
};

