#pragma once
#include "..\Types\Transform.h"
#include <string>
namespace Bases {

    class BaseObject {
        public:
            bool active;
            std::string objectName;
            Types::Transform transform;
            BaseObject(void);
            BaseObject(const std::string &sObjectName);
    };
}

