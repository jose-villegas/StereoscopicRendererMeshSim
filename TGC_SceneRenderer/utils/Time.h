#pragma once
namespace utils {
    // time and framerate has to be calculated by the context provider
    class Time {
        public:
            double deltaTime;
            double totalTime;
            static Time *Instance();
        private:
            static Time *timeInstance;
            Time(void);
    };
}

