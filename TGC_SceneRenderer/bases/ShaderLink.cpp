#include "ShaderLink.h"
using namespace bases;

void bases::ShaderLink::saveUniformLocations(std::vector<unsigned int> locations)
{
    for (unsigned int i = 0; i < locations.size(); i++) {
        this->uniformData[i].uniformLocation = locations[i];
    }
}

bool bases::ShaderLink::saveUniformLocations(std::vector<std::string> names, std::vector<unsigned int> locations)
{
    for (unsigned int i = 0; i < locations.size(); i++) {
        std::string currentName = names[i];
        // Find the uniform name in the uniformLoc vector
        auto it = std::find_if(uniformData.begin(), uniformData.end(),
        [&currentName](const LinkData & index) {
            return index.uniformName == currentName;
        });

        // No uniform with this name so the names param must have wrong values
        if (it == uniformData.end()) { return false; }

        // Set the location to the given variable index found
        uniformData[(*it).variableIndex].uniformLocation = locations[i];
    }

    return true;
}

bases::ShaderLink::ShaderLink(void)
{
}

bases::ShaderLink::LinkData::LinkData(std::string name, unsigned int loc, unsigned int index)
{
    this->uniformLocation = loc;
    this->uniformName = name;
    this->variableIndex = index;
}
