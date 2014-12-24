#pragma once
#include "../types/Transform.h"
#include <string>
namespace bases {

    class BaseObject {
        public:
            bool active;
            std::string objectName;
            types::Transform transform;
            BaseObject(void);
            BaseObject(const std::string &sObjectName);
            virtual void render();
            virtual ~BaseObject() {};
    };
}

