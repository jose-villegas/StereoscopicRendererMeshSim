#pragma once
#include "..\Scene\Camera.h"
#include <vector>

namespace scene {
    class Camera;
}

namespace collections {

    class CamerasCollection {
        private:
            unsigned int activeCamera;
            std::vector<scene::Camera *> cameras;
            static CamerasCollection *instance;
            CamerasCollection(void);
            CamerasCollection(const CamerasCollection &camColl);
        public:
            ~CamerasCollection();
            static CamerasCollection *Instance();
            scene::Camera *createCamera();
            scene::Camera *getCamera(const unsigned int &index);
            void removeCamera(const unsigned int &index);
            void removeCamera(scene::Camera *cam);
            unsigned int cameraCount() const { return cameras.size(); }
            std::vector<scene::Camera *> getCameras() const { return cameras; }
            void setActiveCamera(const unsigned int &index);
            scene::Camera *getActiveCamera();
    };
}

