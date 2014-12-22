#pragma once
#include "BaseObject.h"
namespace bases {

    class BaseComponent {
        public:
            bool alwaysActive;
            BaseObject *base;
            BaseComponent(void);
    };
}

