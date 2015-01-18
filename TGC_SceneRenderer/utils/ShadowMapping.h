#pragma once
#include "..\Scene\Camera.h"
#include "..\Scene\Light.h"
#include "..\core\Matrices.h"
#include "..\types\TextureRenderer.h"
#include <set>

namespace utils {

    class ShadowMapping {
        public:
            enum ShadowQualityPreset {
                VeryLow  = 256,
                Low      = 512,
                Medium   = 1024,
                High     = 2048,
                VeryHigh = 4096,
            };

        private:
            // 0.0 - 1.0, 1 being a pure black shadow
            float shadowStrength;
            // shadow bias matrix factor
            float shadowBias;
            // depth texture and depth projection elemetal matrices
            types::TextureRenderer *depthRenderTexture;
            core::Matrices *matrices;
            // shadow casting lights
            scene::Light *lightSource;
            // light projection pov camera
            scene::Camera *lightPov;

        public:
            // creates a frame buffer object with the given texture quality
            void setup(const ShadowQualityPreset preset);
            void setup(const unsigned int shadowMapSize);
            // adds a light source to rcast shadows from
            void setLightSource(scene::Light *lightSource);
            // binds render target
            void bindMapping();
            // unbinds the render target
            void unbindMapping();

            ShadowMapping(void);
            ~ShadowMapping(void);
    };
}

