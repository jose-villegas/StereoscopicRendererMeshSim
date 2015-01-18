#pragma once
#include "BaseObject.h"
#include <iostream>
namespace bases {

    class BaseComponent : public Renderable {
        public:
            bool alwaysActive;

            BaseObject *base;
            BaseComponent(void);
            virtual ~BaseComponent() {};
    };
}

