#pragma once
#include "BaseObject.h"
#include <iostream>
namespace bases {

    class BaseComponent {
        public:
            bool alwaysActive;

            BaseObject *base;
            BaseComponent(void);
            virtual ~BaseComponent() {};

            // render base method free
            // to implement in components
            virtual void render() {};
    };
}

