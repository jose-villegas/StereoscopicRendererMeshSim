#pragma once
#include "../collections/SceneObjectsCollection.h"
#include "../collections/TexturesCollection.h"
#include "../collections/LightsCollection.h"
#include "../collections/stored/StoredShaders.h"
#include "../scene/Camera.h"
#include "../scene/Mesh.h"
#include "../scene/Light.h"
#include "../types/ShaderProgram.h"
#include "../utils/FrameRate.h"
#include "../utils/Time.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "Matrices.h"
#include "Data.h"

namespace core {

    class Renderer {
        public:
            static Renderer *Instance();
            int load();
            void setup();
            void loop();
        private:
            Renderer(void);
            Renderer(const Renderer &cpy);
            static Renderer *_rdInstance;
            Matrices *_elementalMatrices;
            utils::FrameRate *_frameRate;
            utils::Time *_time;
            collections::LightsCollection *_lights;
            collections::SceneObjectsCollection *_sceneObjects;
    };
}

