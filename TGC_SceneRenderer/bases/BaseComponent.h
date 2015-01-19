#pragma once
#include "BaseObject.h"
#include <iostream>
namespace bases {

    class BaseComponent : public Renderable {
        public:
            bool active;

            BaseObject *base;
            BaseComponent(void);
            virtual ~BaseComponent() {};
    };
}

