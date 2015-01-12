#pragma once
#include "../collections/LightsCollection.h"
#include "../types/ShaderProgram.h"
#include "../bases/BaseComponent.h"
#include "../bases/ShaderLinks.h"
#include "../core/Data.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <unordered_map>
#include <utility>

namespace collections {
    class LightsCollection;  // Forward Declare to solve circular dependency
}

namespace scene {

    class Light : public bases::BaseComponent {

        public:

            static enum LightType {
                Point,
                Spot,
                Directional
            };

        private:
            friend class collections::LightsCollection;
            // Only Lights Collection can create Light Objects
            Light(const Light &lght);
            Light(const LightType &lghtType);
            Light(void);
            ~Light(void);

        public:

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
            // light direction based on rotation and origin position with up vector
            glm::vec3 getDirection();

            float setCosInnerConeAngle() { return cosInnerConeAngle = std::cos(innerConeAngle); }
            float setCosOuterConeAngle() { return cosOuterConeAngle = std::cos(outerConeAngle); }

            static std::string getLightTypeString(const LightType &lightType);
    };
}

