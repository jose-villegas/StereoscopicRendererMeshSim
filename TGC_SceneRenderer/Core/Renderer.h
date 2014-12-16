#pragma once
#include "..\Types\ShaderProgram.h"
#include "..\Scene\Camera.h"
#include "..\Utils\FrameRate.h"
#include "..\Utils\Time.h"
namespace Core {

    class Renderer {
        public:
            Renderer(void);
            bool load();
            void setup();
            void loop();
        private:
            Utils::FrameRate *frameRate;
            Utils::Time *time;
    };
}

