#pragma once
#include "../bases/BaseComponent.h"
#include "../bases/ShaderLink.h"
#include "../core/Data.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../types/ShaderProgram.h"
#include <unordered_map>
#include <utility>
// Number of members in the GLSL Structure
#define GLSL_LIGHT_MEMBER_COUNT 8

namespace scene {

    class Light : public bases::BaseComponent, public bases::ShaderLink {
        private:
            Light(const Light &lght);
        public:
            Light(void);

            enum LightType {
                Directional,
                Spot,
                Point
            };

            glm::vec3 color;
            float intensity;
            float attenuation;
            // Spot Light Params
            float innerConeAngle;
            float outerConeAngle;
            LightType lightType;

            void setUniforms(types::ShaderProgram *shp);
    };
}

