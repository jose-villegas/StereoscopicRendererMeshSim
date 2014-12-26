#include "FrameRate.h"
using namespace utils;

FrameRate::FrameRate(void)
{
    fpsSamples.fill(0);
    currentFramerate = 0;
    currentSample = 0;
}

void utils::FrameRate::calculate(const double &deltaTime)
{
    if (deltaTime == 0.0) { return; }

    this->currentFramerate = (1.0 / deltaTime) * 0.9 + this->currentFramerate * 0.1;
}

FrameRate *utils::FrameRate::Instance()
{
    if (!framerateInstance) {
        framerateInstance = new FrameRate();
    }

    return framerateInstance;
}

FrameRate *utils::FrameRate::framerateInstance = nullptr;
