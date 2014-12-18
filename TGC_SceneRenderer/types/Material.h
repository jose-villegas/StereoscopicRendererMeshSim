#pragma once
#include "../bases/ShaderLink.h"
#include "../core/Data.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>
namespace types {

    class Material : public bases::ShaderLink {
        public:
            // Supported Material Shaders
            enum MaterialType {
                Diffuse,
                BumpedDiffuse
            };

        private:
            std::vector<Texture *> _textures;
            ShaderProgram *_matShader;
            MaterialType _matType;
            Material(const Material &mat);

        public:
            Material(void);
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            float shininess;

            void addTexture(Texture *tex);
            void setShaderProgram(ShaderProgram *shp);
            void setMaterialType();
    };
}

