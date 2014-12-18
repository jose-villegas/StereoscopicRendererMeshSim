#pragma once
#include "BaseObject.h"
namespace bases {

    class BaseComponent : public BaseObject {
        public:
            bool alwaysActive;
            bool active;
            BaseComponent(void);
    };
}

