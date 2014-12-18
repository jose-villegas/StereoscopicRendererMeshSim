#pragma once
#include <string>
namespace core {

    class Constrains {
        public:
            static const int MAX_LIGHTS = 4;
            /*
             * Parses de class variables to GLSL data
             * Method:    toGLSL
             * FullName:  core::Constrains::toGLSL
             * Access:    public static
             * Returns:   std::string
             */
            static const std::string ToGLSL();
            ~Constrains();
    };
}

