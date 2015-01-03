#pragma once

#include "../collections/SceneObjectsCollection.h"
#include "../collections/TexturesCollection.h"
#include "../utils/FrameRate.h"
#include "../utils/Time.h"
#include "Matrices.h"

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
            void unload();
            void loop();
            void viewport(const unsigned int &width, const unsigned int &height);
            void polygonModel(Modes mode);
            ~Renderer();

        private:
            // camera has access to renderer collections
            friend class scene::Camera;
            // constructor private for single instance
            Renderer(void);
            Renderer(const Renderer &cpy);
            static Renderer *instance;
            Matrices *matrices;
            collections::CamerasCollection *cameras;
            collections::LightsCollection *lights;
            collections::MeshesCollection *meshes;
            collections::SceneObjectsCollection *sceneObjects;
            collections::TexturesCollection *textures;
            scene::Camera *activeCamera;
            utils::FrameRate *framerate;
            utils::Time *time;

    };
}

