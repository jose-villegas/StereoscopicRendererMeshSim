#pragma once
#include "../bases/ShaderLinks.h"
#include "../core/Data.h"
#include "GL/gl_core_4_4.h"
namespace collections {

    class LightsCollection {
        private:
            types::ShaderProgram::UniformBlock *_uniformBlockInfo;
            // Class Instance
            static LightsCollection *_lightsCollection;

            LightsCollection(void);
            LightsCollection(const LightsCollection &lightc);
        public:
            static LightsCollection *Instance();
    };
}

