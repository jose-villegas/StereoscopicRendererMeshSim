#pragma once
#include "../bases/ShaderLinks.h"
#include "../core/Data.h"
#include "../Scene/Light.h"
#include <vector>

namespace collections {

    class LightsCollection : public bases::ShaderLinkBlock {
        private:
            // Class Instance
            static LightsCollection *instance;
            std::vector<scene::Light *> lights;
            LightsCollection(void);
            LightsCollection(const LightsCollection &lightc);
            // holder to modelviewMatrix convert light position to eyecords
            glm::mat4 viewMatrix;

        public:
            ~LightsCollection();
            static LightsCollection *Instance();
            void setViewMatrix(const glm::mat4 &viewMat) { this->viewMatrix = viewMat; };
            void setUniformBlock();
            void setUniformBlockInfo();
            void removeLight(const unsigned int &lightIndex);
            void removeLight(scene::Light *lght);
            const unsigned int lightCount() const;
            scene::Light *createLight(const unsigned int lightType);
            scene::Light *getLight(const unsigned int &lightIndex) const;
            std::vector<scene::Light *> getLights() const { return lights; }

    };
}

