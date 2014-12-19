#pragma once
#include "../types/ShaderProgram.h"
#include "../bases/ShaderLink.h"
#include "glm/detail/type_mat.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "Data.h"

namespace core {
    // Elemental Matrices
    class Matrices : public bases::ShaderLink {
        private:
            types::ShaderProgram::UniformBlock *_uniformBlockInfo;
            types::ShaderProgram utilitySHP;
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 projection;
            glm::mat4 modelViewProjection;
            glm::mat4 modelView;
            glm::mat4 normal;

        public:
            Matrices(void);

            /*
             * Calculates modelViewProjection
             * modelView and normal matrices
             */
            void calculateMatrices();
            void setViewMatrix(const glm::mat4 &value);
            void setModelMatrix(const glm::mat4 &value);
            void setUniforms(types::ShaderProgram *shp);
            // only use this if uniformBlockInfo is set
            void setUniforms();
            void setProjectionMatrix(const glm::mat4 &value);
            void setUniformBlockInfo(types::ShaderProgram::UniformBlock *uniformBlockInfo);
            // only use this if uniformBlockInfo is set
            void setUniformBlockInfoIndicesAndOffsets();
    };
}

