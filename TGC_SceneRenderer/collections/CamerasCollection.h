#pragma once
#include "..\Scene\Camera.h"
#include <vector>

namespace scene {
    class Camera;
}

namespace collections {

    class CamerasCollection {
        private:
            unsigned int _activeCamera;
            std::vector<scene::Camera *> _cameras;
            static CamerasCollection *_instance;
            CamerasCollection(void);
            CamerasCollection(const CamerasCollection &camColl);
        public:
            static CamerasCollection *Instance();
            scene::Camera *createCamera();
            scene::Camera *getCamera(const unsigned int &index);
            void removeCamera(const unsigned int &index);
            void removeCamera(scene::Camera *cam);
            unsigned int cameraCount() const { return _cameras.size(); }
            std::vector<scene::Camera *> getCameras() const { return _cameras; }
            void setActiveCamera(const unsigned int &index);
            scene::Camera *getActiveCamera();
    };
}

