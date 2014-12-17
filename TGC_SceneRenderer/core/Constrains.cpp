#include "Constrains.h"
using namespace core;

std::string const core::Constrains::ToGLSL()
{
    std::string res = "\nconst int MAX_LIGHTS = " + std::to_string(MAX_LIGHTS) + ";\n";
    return res;
}