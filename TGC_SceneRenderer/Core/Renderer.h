#pragma once
#include "../types/ShaderProgram.h"
#include "../scene/Camera.h"
#include "../utils/FrameRate.h"
#include "../utils/Time.h"
#include "../scene/Mesh.h"
#include "../collections/Textures.h"
#include "glm/gtx/transform.hpp"
#include "ResourcesLoader.h"

namespace core {

    class Renderer {
        public:
            Renderer(void);
            bool load();
            void setup();
            void loop();
        private:
            utils::FrameRate *frameRate;
            utils::Time *time;
            collections::Textures *_texCollection;
    };
}

