#pragma once
#include "..\Scene\Camera.h"
#include "..\Scene\Light.h"
#include "..\Types\ShaderProgram.h"
#include "..\bases\ShaderLinks.h"
#include "..\core\Matrices.h"
#include "..\types\TextureRenderer.h"
#include <set>

namespace scene {
    class Light;
}

namespace utils {

    class ShadowMapping : public bases::ShaderLinkBlock {
        public:
            enum ShadowQualityPreset : unsigned int {
                VeryLow  = 256,
                Low      = 512,
                Medium   = 1024,
                High     = 2048,
                VeryHigh = 4096,
            };

        private:

            // 0.0 - 1.0, 1 being a pure black shadow
            glm::vec3 shadowStrength;
            // depth texture and depth projection elemetal matrices
            types::TextureRenderer *depthRenderTexture;
            core::Matrices *matrices;
            // shadow casting lights
            scene::Light *lightSource;
            // light projection pov camera
            scene::Camera *lightPov;
            // shadow size quality
            unsigned int shadowMapSize;
            // light projectors associated index
            unsigned int lightProjectorIndex;
            // bool shadow mapping
            bool shadowMappingEnabled;

        public:

            float shadowProjectionFarDistance;
            // creates a frame buffer object with the given texture quality
            bool setup(const ShadowQualityPreset preset);
            bool setup(const unsigned int shadowMapSize);
            // adds a light source to rcast shadows from
            void setLightSource(scene::Light *lightSource);
            // binds render target
            void bindMapping();
            // unbinds the render target
            void unbindMapping();
            // map to depth texture
            void shadowRenderPass();
            // Sets uniform block data, needs uniformBlock info and sLinkSP to be set
            void setUniformBlock();
            // sets the uniformblockinfo based on the stored shaderprogram , sLinkSP
            // needs to be set call this function only once
            void setUniformBlockInfo();
            // for depth projection matrices
            core::Matrices *getMatrices() const { return matrices; }
            // binds shadow map textures as active, shadow map indexes start from types::Texture::TextureType::Count
            void bindShadowMapTextures();
            // sets shadow map uniforms to the provided shader
            void setTextureMapUniform(types::ShaderProgram *shp);

            ShadowMapping(void);
            ~ShadowMapping(void);
    };
}

