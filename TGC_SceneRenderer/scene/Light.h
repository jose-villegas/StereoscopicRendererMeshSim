#pragma once
#include "../bases/BaseComponent.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../types/ShaderProgram.h"
#include <unordered_map>
#include <utility>
// Number of members in the GLSL Structure
#define GLSL_LIGHT_MEMBER_COUNT 8

namespace scene {

    class Light : public bases::BaseComponent {
        private:
            // Check shared_data.glsl for values order
            std::vector< unsigned int> _uniformLoc;
            Light(const Light &light);
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
            bool uniformsSet;
            void saveUniformLocations(const unsigned int locations[GLSL_LIGHT_MEMBER_COUNT]);
            void saveUniformLocations(const std::string names[GLSL_LIGHT_MEMBER_COUNT], const unsigned int locations[GLSL_LIGHT_MEMBER_COUNT]);
            void setUniforms(types::ShaderProgram *sh);
    };
}

