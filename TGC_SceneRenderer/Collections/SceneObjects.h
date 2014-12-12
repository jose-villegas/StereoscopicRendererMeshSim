#pragma once
namespace ECollections {

    class SceneObjects {
        public:
            static SceneObjects *Instance();
            SceneObjects(void);
            ~SceneObjects();
        private:
            static SceneObjects *_eInstance;
    };

}
