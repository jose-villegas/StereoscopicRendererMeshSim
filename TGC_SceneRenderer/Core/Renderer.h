#pragma once
#include "..\Types\ShaderProgram.h"
#include "..\Scene\Camera.h"

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

