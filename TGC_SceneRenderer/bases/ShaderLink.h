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
            virtual bool saveUniformLocations(const types::ShaderProgram *shp);
            virtual void setUniforms(types::ShaderProgram *shp) = 0;
    };
}

