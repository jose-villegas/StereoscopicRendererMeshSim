#pragma once
#include "../bases/ShaderLinks.h"
#include "../scene/Light.h"
#include "../core/Data.h"
#include "GL/gl_core_4_4.h"

namespace collections {

    class LightsCollection : public bases::ShaderLinkBlock {
        private:
            // Class Instance
            static LightsCollection *_lightsCollection;
            std::vector<scene::Light *> _sceneLights;

            LightsCollection(void);
            LightsCollection(const LightsCollection &lightc);
        public:
            static LightsCollection *Instance();
            void setUniformBlock();
            void setUniformBlockInfo();

            scene::Light *createLight();
            void removeLight(const unsigned int &lightIndex);
            const scene::Light *getLight(const unsigned int &lightIndex);
    };
}

