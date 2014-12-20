#pragma once
#include "../types/ShaderProgram.h"
#include "../bases/ShaderLink.h"
#include "../Collections/stored/Shaders.h"
#include "glm/detail/type_mat.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "Data.h"

namespace core {
    // Elemental Matrices
    class Matrices : public bases::ShaderLink {
        private:
            types::ShaderProgram::UniformBlock *_uniformBlockInfo;
            types::ShaderProgram *_utilitySHP;
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 projection;
            glm::mat4 modelViewProjection;
            glm::mat4 modelView;
            glm::mat4 normal;

        public:
            Matrices(void);
            ~Matrices();
            /*
             * Calculates modelViewProjection
             * modelView and normal matrices
             */
            void calculateMatrices();
            void setViewMatrix(const glm::mat4 &value);
            void setModelMatrix(const glm::mat4 &value);
            // slower than setting the uniform block info and offsets
            // and using simply setuniforms() but works with any shaderprogram
            void setUniforms(types::ShaderProgram *shp);
            // only use this if uniformBlockInfo is set
            void setUniforms();
            void setProjectionMatrix(const glm::mat4 &value);
            // sets the class holder for uniform info and saves the uniform block info indices and offsets
            void setUniformBlockInfo();

    };
}

