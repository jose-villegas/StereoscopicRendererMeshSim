#pragma once
#include "../types/Transform.h"
#include "Renderable.h"
#include <string>
namespace bases {

    class BaseObject : public Renderable {
        public:
            bool active;
            std::string objectName;
            types::Transform transform;

            BaseObject(void);
            BaseObject(const std::string &sObjectName);
            virtual ~BaseObject() {};
    };
}

