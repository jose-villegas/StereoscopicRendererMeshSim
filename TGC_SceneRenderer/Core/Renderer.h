#pragma once
#include "GL/gl_core_4_4.h"
#include "GL/wglext.h"
namespace Core {

    class Renderer {
        public:
            Renderer(void);
            bool load();
            void setup();
            void loop();
        private:

    };
}

