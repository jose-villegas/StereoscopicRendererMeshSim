#pragma once
#include "../bases/ShaderLinks.h"
#include "../collections/stored/StoredShaders.h"
#include "../core/Data.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include <utility>

namespace types {

    class Material : public bases::ShaderLink {
        private:
            std::vector<Texture *> textures;
            std::vector<bool> shaderTextures;
            std::vector<unsigned int> hasTextureType;
            std::vector<std::pair<unsigned int, Texture::TextureType>> activeShaderTextures;
            ShaderProgram *matShader;
            Material(const Material &mat);

        public:

            Material(void);
            ~Material(void);

            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            glm::vec3 emission;
            int shadingModel;
            float shininess;

            // Assigns a guessed ShaderProgram from the stored shaders
            // based on the stored textures and their type, for example
            // a material with diffuse and normal maps would be assigned
            // a BumpedDiffuse ShaderProgram
            void guessMaterialShader();
            // Adds textures, either a single texture or many textures pass in an array
            void addTexture(Texture *tex);
            void addTexture(Texture *tex, types::Texture::TextureType texType);
            void setShaderProgram(ShaderProgram *shp);
            // matShader needs to be set
            void useMaterialShader();
            void bindTextures() const;
            // Pass and binds the textures ID to the ShaderProgram
            void setTexturesUniforms(types::ShaderProgram *shp);
            // Uses the class member stored ShaderProgram _matShader
            void setTexturesUniforms();
            // Implemented pure virtual method, check ShaderLink class
            // sets the shaderprogram uniform data and
            // sets the stored textures uniforms
            void setUniforms(types::ShaderProgram *shp);
            // Uses the class member stored ShaderProgram _matShader
            // sets the shaderprogram uniform data and
            // sets the stored textures uniforms
            void setUniforms();
            // uses assimp pMaterial structure to load relevant
            // parameters data
            void loadMaterialValues(const aiMaterial *aiMat);

            unsigned int textureCount() const { return textures.size(); };

            bool isUsingTextureType(types::Texture::TextureType texType) { return (unsigned int)texType < this->shaderTextures.size() ? this->shaderTextures[texType] : false; };
    };
}

