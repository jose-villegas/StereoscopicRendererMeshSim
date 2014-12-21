#pragma once
#include "..\types\ShaderProgram.h"
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>
namespace bases {

    class ShaderLink {
        private:
            ShaderLink(const ShaderLink &cpy);
        protected:
            ShaderLink(void);

            struct LinkData {
                LinkData(std::string name, unsigned int loc, unsigned int index);
                std::string uniformName;
                unsigned int uniformLocation;
                unsigned int variableIndex;
            };

            // Vector of struct < uniform name, uniform location, variable index >
            // the variable index indicates which object member is related to the uniform name
            // 0 being the first struct member n the last, check shared_data.glsl and core::Data
            // to see the structs's members order, uniformLoc is ordered
            // by variable index. Resize this vector accordly before using
            std::vector<LinkData> uniformData;
            // Asumes the locations are ordered by shared_data.glsl structures member order
            virtual void saveUniformLocations(std::vector<unsigned int> locations);
            virtual bool saveUniformLocations(std::vector<std::string> names, std::vector<unsigned int> locations);
            // Saves the uniform locations searching in the shader program
            virtual bool saveUniformLocations(const types::ShaderProgram *shp);
            // Sets the class uniform to the shader program
            virtual void setUniforms(types::ShaderProgram *shp) = 0;
            // Function to set shader uniforms if _utilitySHP is set
            virtual void setUniforms() = 0;
    };

    class ShaderLinkBlock {
        private:
            ShaderLinkBlock(const ShaderLinkBlock &slb);
        protected:
            ShaderLinkBlock();
            types::ShaderProgram::UniformBlock *_uniformBlockInfo;
            types::ShaderProgram *_sLinkSP;
            // Sets uniform block data, needs uniformBlock info and sLinkSP to be set
            virtual void setUniformBlock() = 0;
            // sets the uniformblock info based on the stored shaderprogram , sLinkSP
            // needs to be set
            virtual void setUniformBlockInfo() = 0;
            // binds the ubo
            void bindUniformBuffer();
            // updates the data in the ubo
            void updateUniformBufferData();
        public:
            // sets the shaderprogram
            virtual void setShaderProgram(types::ShaderProgram *shp);
    };
}

