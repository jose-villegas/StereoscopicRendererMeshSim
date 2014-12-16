#pragma once
#include "..\Types\ShaderProgram.h"
#include "..\Scene\Camera.h"
#include "..\Utils\FrameRate.h"
#include "..\Utils\Time.h"
#include "..\Scene\Mesh.h"
#include "..\Collections\Textures.h"
#include "glm\gtx\transform.hpp"

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
            ECollections::Textures *_texCollection;
    };
}

