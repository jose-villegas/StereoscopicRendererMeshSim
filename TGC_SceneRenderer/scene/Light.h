#pragma once
#include "../bases/BaseComponent.h"
#include "../bases/ShaderLinks.h"
#include "../core/Data.h"
#include "../types/ShaderProgram.h"
#include "../utils/ShadowMapping.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <array>

namespace utils {
    class ShadowMapping;
}

namespace scene {


    class Light : public bases::BaseComponent {

        public:

            static glm::vec3 ambientLight;

            static enum LightType {
                Point,
                Spot,
                Directional
            };

        private:
            // shadowing params static shared between all lights
            static std::array<utils::ShadowMapping *, core::EngineData::Constrains::MAX_SHADOWMAPS> shadowProjector;
            static unsigned int shadowProjectorCount;
            // shadowing params specific per class instance
            bool projectShadows;
            int shadowProjectorIndex;

            Light(const Light &lght);

        public:

            Light(void);
            Light(const LightType &lghtType);
            ~Light(void);

            glm::vec3 color;
            float intensity;
            float attenuation;
            // spot light members
            // params in radians
            float innerConeAngle;
            float outerConeAngle;
            float cosInnerConeAngle;
            float cosOuterConeAngle;

            LightType lightType;
            // Values 0.0 - 1.0
            void setColor(const float &value0, const float &value1, const float &value2);
            // Values 0 - 255
            void setColor(const unsigned int &value0, const unsigned int &value1, const unsigned int &value2);
            // light direction based on rotation and origin position with forward vector
            glm::vec3 getDirection();
            // set if this light casts shadows
            void enableShadowProjection(bool value);
            // gets the light associated projection index
            int getShadowProjectorIndex() const { return shadowProjectorIndex; }
            // get the light's associated shadow projector
            utils::ShadowMapping *getShadowProjector() { return shadowProjector[this->shadowProjectorIndex]; };
            // gets all the shadow projectors, null pointer means no shadow casting
            static const std::array<utils::ShadowMapping *, core::EngineData::Constrains::MAX_SHADOWMAPS> &getShadowProjectors() { return shadowProjector; }

            float setCosInnerConeAngle() { return cosInnerConeAngle = std::cos(innerConeAngle); }
            float setCosOuterConeAngle() { return cosOuterConeAngle = std::cos(outerConeAngle); }

            static std::string getLightTypeString(const LightType &lightType);
            // returns if there is any light projecting shadows
            static const unsigned int &getShadowCount() { return shadowProjectorCount; };
    };
}

