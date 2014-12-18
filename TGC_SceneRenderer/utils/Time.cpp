#include "Time.h"
using namespace utils;

Time *utils::Time::timeInstance = nullptr;

Time *utils::Time::Instance()
{
    if (!timeInstance) {
        timeInstance = new Time();
    }

    return timeInstance;
}

utils::Time::Time(void)
{
    this->_deltaTime = 0;
    this->_totalTime = 0;
}
