#pragma once
#include "..\Scene\Camera.h"
#include <vector>

namespace scene {
    class Camera;
}

namespace collections {

    class CamerasCollection {
        private:
            std::vector<scene::Camera *> _cameras;
            static CamerasCollection *_instance;
            CamerasCollection(void);
            CamerasCollection(const CamerasCollection &camColl);
        public:
            static CamerasCollection *Instance();
            scene::Camera *createCamera();
            scene::Camera *getCamera(const unsigned int &index);
            void removeCamera(const unsigned int &index);
    };
}

