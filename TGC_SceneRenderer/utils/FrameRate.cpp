#include "FrameRate.h"
using namespace utils;

FrameRate::FrameRate(void)
{
    currentFramerate = 0;
}

void utils::FrameRate::calculate(const double &deltaTime)
{
    if (deltaTime == 0.0) { return; }

    this->currentFramerate = (1.0 / deltaTime) * 0.9 + this->currentFramerate * 0.1; // smoothing
}

FrameRate *utils::FrameRate::Instance()
{
    if (!framerateInstance) {
        framerateInstance = new FrameRate();
    }

    return framerateInstance;
}

FrameRate *utils::FrameRate::framerateInstance = nullptr;
