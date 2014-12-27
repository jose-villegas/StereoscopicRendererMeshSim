#pragma once
#include <array>

namespace utils {
    // time and framerate has to be calculated by the context provider
    class FrameRate {
        private:
            static FrameRate *framerateInstance;
            double currentFramerate;
        public:
            double getCurrentFramerate() { return currentFramerate; };
            static FrameRate *Instance();
            // Calculates the average framerate based on NUM_FPS_SAMPLES
            void calculate(const double &deltaTime);
        protected:
            FrameRate(void);
    };
}