#pragma once

namespace Utils {
    ref class FrameRate {
        private:
            static System::Int32 const NUM_FPS_SAMPLES = 32;
            array<System::Double> ^fpsSamples;
            System::Int32 currentSample;
            System::Double frameTimeSum;
        public:
            FrameRate(void);
            ~FrameRate();
            // Calculates the average framerate based on NUM_FPS_SAMPLES
            System::Double Calculate(System::Double deltaTime);
    };
}