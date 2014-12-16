#include "FrameRate.h"
using namespace Utils;

FrameRate::FrameRate(void)
{
    fpsSamples.fill(0);
    _currentFramerate = 0;
    currentSample = 0;
    frameTimeSum = 0.0;
    filled = false;
}

void Utils::FrameRate::calculate(const double &deltaTime)
{
    if (deltaTime == 0.0) { return; }

    this->_currentFramerate = (1.0 / deltaTime) * 0.9 + this->_currentFramerate * 0.1;
}

FrameRate *Utils::FrameRate::Instance()
{
    if (!framerateInstance) {
        framerateInstance = new FrameRate();
    }

    return framerateInstance;
}

FrameRate *Utils::FrameRate::framerateInstance = nullptr;
