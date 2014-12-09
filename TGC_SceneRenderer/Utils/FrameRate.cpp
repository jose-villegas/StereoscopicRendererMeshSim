#include "FrameRate.h"


Utils::FrameRate::FrameRate(void)
{
    fpsSamples = gcnew array<System::Double>(NUM_FPS_SAMPLES);
    currentSample = 0;
    frameTimeSum = 0.0;
}

System::Double Utils::FrameRate::Calculate(System::Double deltaTime)
{
    if (deltaTime == 0.0) { return 0.0; }

    System::Double currentFrameRate = 1.0 / deltaTime;
    frameTimeSum -= fpsSamples[currentSample];
    frameTimeSum += currentFrameRate;
    fpsSamples[currentSample] = currentFrameRate;

    if (++currentSample == NUM_FPS_SAMPLES) {
        currentSample = 0;
    }

    return ((System::Double)frameTimeSum / NUM_FPS_SAMPLES);
}
