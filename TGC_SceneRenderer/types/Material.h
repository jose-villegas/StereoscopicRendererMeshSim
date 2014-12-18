#pragma once
#include "../bases/ShaderLink.h"
#include "../core/Data.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include <set>
namespace types {

    class Material : public bases::ShaderLink {
        private:
            struct TexturePtrComp {
                bool operator()(const Texture *lhs, const Texture *rhs) const  { return lhs->getType() < rhs->getType(); }
            };
            // Ordered Textures by type check Texture.h
            std::set<Texture *, TexturePtrComp> _textures;
            ShaderProgram *_matShader;
            Material(const Material &mat);

        public:
            Material(void);
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            float shininess;

            void addTexture(Texture *tex);
            void addTexture(Texture *tex[], const unsigned int texCount);
            void setShaderProgram(ShaderProgram *shp);
            void setUniforms(types::ShaderProgram *shp);
    };
}

