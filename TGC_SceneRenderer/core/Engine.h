#pragma once

#include "../collections/SceneObjectsCollection.h"
#include "../collections/TexturesCollection.h"
#include "../utils/FrameRate.h"
#include "../utils/Time.h"
#include "Matrices.h"

namespace core {

    class Engine {

        public:

            enum Modes {
                Points = GL_POINT,
                Wireframe = GL_LINE,
                Textured = GL_FILL,
            };

            static Engine *Instance();

            int load();
            void setup();
            void unload();
            void loop();
            void viewport(const unsigned int width, const unsigned int height);
            void polygonModel(Modes mode);
            void meshRenderFlags(bool position, bool uvs, bool normals, bool tangents, bool bitangets);
            ~Engine();

        private:
            // camera has access to renderer collections
            friend class scene::Camera;
            // constructor private for single instance
            Engine(void);
            Engine(const Engine &cpy);

            static Engine *instance;

            Matrices *matrices;
            collections::CamerasCollection *cameras;
            collections::LightsCollection *lights;
            collections::MeshesCollection *meshes;
            collections::SceneObjectsCollection *sceneObjects;
            collections::TexturesCollection *textures;
            scene::Camera *activeCamera;
            utils::FrameRate *framerate;
            utils::Time *time;

            bool initialized;

    };
}

