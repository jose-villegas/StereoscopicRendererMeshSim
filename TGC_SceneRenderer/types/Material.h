#pragma once
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>
namespace types {

    class Material {
        public:

            // Supported Material Shaders
            enum MaterialType {
                Diffuse,
                BumpedDiffuse
            };

            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            Material(void);
        private:
            std::vector<Texture *> _textures;
            ShaderProgram *_matShader;
            MaterialType _matType;
    };
}

