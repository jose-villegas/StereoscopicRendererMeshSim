#include "LightsCollection.h"
using namespace collections;

LightsCollection::LightsCollection(void)
{
}

LightsCollection *collections::LightsCollection::Instance()
{
    if (!_lightsCollection) {
    }

    return _lightsCollection;
}

LightsCollection *collections::LightsCollection::_lightsCollection;
