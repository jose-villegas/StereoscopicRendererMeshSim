#pragma once
#include "Matrices.h"
#include "../Utils/FrameRate.h"
#include "../Utils/Time.h"
#include "../collections/LightsCollection.h"
#include "../Collections/SceneObjectsCollection.h"
#include "../collections/MeshesCollection.h"
#include "../collections/CamerasCollection.h"

namespace core {



    class Renderer {

        public:

            static Renderer *Instance();
            int load();
            void setup();
            void loop();
            void viewport(const unsigned int &width, const unsigned int &height);

        private:

            Renderer(void);
            Renderer(const Renderer &cpy);
            static Renderer *_rdInstance;
            Matrices *_elementalMatrices;
            utils::FrameRate *_frameRate;
            utils::Time *_time;
            scene::Camera *_activeCamera;
            collections::LightsCollection *_lights;
            collections::MeshesCollection *_meshes;
            collections::CamerasCollection *_cameras;
            collections::SceneObjectsCollection *_sceneObjects;

    };
}

