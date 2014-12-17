#pragma once
#include "../collections/Textures.h"
#include "../collections/stored/Shaders.h"
#include "../scene/Camera.h"
#include "../scene/Mesh.h"
#include "../scene/Light.h"
#include "../types/ShaderProgram.h"
#include "../utils/FrameRate.h"
#include "../utils/Time.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
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

