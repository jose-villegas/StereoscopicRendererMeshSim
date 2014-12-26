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

            enum Modes {
                Points = GL_POINT,
                Wireframe = GL_LINE,
                Textured = GL_FILL,
            };

            static Renderer *Instance();
            int load();
            void setup();
            void loop();
            void viewport(const unsigned int &width, const unsigned int &height);
            void polygonModel(Modes mode);

        private:

            Renderer(void);
            Renderer(const Renderer &cpy);
            static Renderer *instance;
            Matrices *matrices;
            collections::CamerasCollection *cameras;
            collections::LightsCollection *lights;
            collections::MeshesCollection *meshes;
            collections::SceneObjectsCollection *sceneObjects;
            scene::Camera *activeCamera;
            utils::FrameRate *framerate;
            utils::Time *time;

    };
}

