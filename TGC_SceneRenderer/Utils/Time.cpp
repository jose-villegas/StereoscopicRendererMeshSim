#include "Time.h"
using namespace Utils;

Time *Utils::Time::timeInstance = nullptr;

Time *Utils::Time::Instance()
{
    if (!timeInstance) {
        timeInstance = new Time();
    }

    return timeInstance;
}

Utils::Time::Time(void)
{
    this->_deltaTime = 0;
    this->_totalTime = 0;
}
